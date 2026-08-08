#include "http.hpp"

#import <Foundation/Foundation.h>

#include <algorithm>
#include <chrono>
#include <string_view>
#include <utility>

namespace dusk::http {
namespace {
bool valid_redirect_url(NSURL* url);
bool same_origin(NSURL* first, NSURL* second);
bool valid_response_headers(NSDictionary* headers);
std::string to_string(NSString* value);
bool expired(const std::chrono::steady_clock::time_point deadline);
NSTimeInterval remaining_timeout(const std::chrono::steady_clock::time_point deadline);
dispatch_time_t timeout_deadline(const std::chrono::steady_clock::time_point deadline);
NSMutableURLRequest* owned_request(NSURL* url, Method method, const std::string& body,
    const std::vector<Header>& headers, const std::chrono::steady_clock::time_point deadline);
void apply_redirect_policy(Method& method, std::string& body, std::vector<Header>& headers,
    NSInteger status, bool crossOrigin);
}  // namespace
}  // namespace dusk::http

@interface DuskHttpRequestDelegate : NSObject <NSURLSessionDataDelegate, NSURLSessionTaskDelegate> {
    dusk::http::Method _method;
    std::string _body;
    std::vector<dusk::http::Header> _headers;
    NSURL* _currentURL;
    std::chrono::steady_clock::time_point _deadline;
}
@property(nonatomic) dispatch_semaphore_t semaphore;
@property(nonatomic) size_t maxBodyBytes;
@property(nonatomic, strong) NSMutableData* data;
@property(nonatomic, strong) NSURLResponse* response;
@property(nonatomic, strong) NSError* error;
@property(nonatomic) BOOL tooLarge;
@property(nonatomic) BOOL malformed;
@property(nonatomic) BOOL timedOut;
@property(nonatomic) NSUInteger redirects;
- (instancetype)initWithRequest:(const dusk::http::Request&)request initialURL:(NSURL*)url
                        deadline:(std::chrono::steady_clock::time_point)deadline;
- (NSMutableURLRequest*)initialRequest;
@end

@implementation DuskHttpRequestDelegate

- (instancetype)initWithRequest:(const dusk::http::Request&)request initialURL:(NSURL*)url
                        deadline:(const std::chrono::steady_clock::time_point)deadline {
    self = [super init];
    if (self != nil) {
        _semaphore = dispatch_semaphore_create(0);
        _maxBodyBytes = request.maxBodyBytes;
        _data = [NSMutableData data];
        _method = request.method;
        _body = request.body;
        _headers = request.headers;
        _currentURL = url;
        _deadline = deadline;
    }
    return self;
}

- (NSMutableURLRequest*)initialRequest {
    return dusk::http::owned_request(_currentURL, _method, _body, _headers, _deadline);
}

- (void)URLSession:(NSURLSession*)session
              task:(NSURLSessionTask*)task
    willPerformHTTPRedirection:(NSHTTPURLResponse*)response
                    newRequest:(NSURLRequest*)request
              completionHandler:(void (^)(NSURLRequest*))completionHandler {
    if (dusk::http::expired(_deadline)) {
        self.timedOut = YES;
        completionHandler(nil);
        return;
    }
    if (self.redirects++ >= 5 || !dusk::http::valid_response_headers(response.allHeaderFields) ||
        !dusk::http::valid_redirect_url(request.URL))
    {
        self.malformed = YES;
        completionHandler(nil);
        return;
    }
    const bool crossOrigin = !dusk::http::same_origin(_currentURL, request.URL);
    dusk::http::apply_redirect_policy(_method, _body, _headers, response.statusCode, crossOrigin);
    _currentURL = request.URL;
    NSMutableURLRequest* redirected =
        dusk::http::owned_request(_currentURL, _method, _body, _headers, _deadline);
    if (redirected == nil) {
        self.timedOut = YES;
        completionHandler(nil);
        return;
    }
    completionHandler(redirected);
}

- (void)URLSession:(NSURLSession*)session
          dataTask:(NSURLSessionDataTask*)dataTask
didReceiveResponse:(NSURLResponse*)response
 completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler {
    if (dusk::http::expired(_deadline)) {
        self.timedOut = YES;
        completionHandler(NSURLSessionResponseCancel);
        return;
    }
    if ([response isKindOfClass:[NSHTTPURLResponse class]] &&
        !dusk::http::valid_response_headers([(NSHTTPURLResponse*)response allHeaderFields])) {
        self.malformed = YES;
        completionHandler(NSURLSessionResponseCancel);
        return;
    }
    self.response = response;
    completionHandler(NSURLSessionResponseAllow);
}

- (void)URLSession:(NSURLSession*)session
          dataTask:(NSURLSessionDataTask*)dataTask
    didReceiveData:(NSData*)data {
    if (dusk::http::expired(_deadline)) {
        self.timedOut = YES;
        [dataTask cancel];
        return;
    }
    if (data.length > self.maxBodyBytes || self.data.length > self.maxBodyBytes - data.length) {
        self.tooLarge = YES;
        [dataTask cancel];
        return;
    }
    [self.data appendData:data];
    if (dusk::http::expired(_deadline)) {
        self.timedOut = YES;
        [dataTask cancel];
    }
}

- (void)URLSession:(NSURLSession*)session
                    task:(NSURLSessionTask*)task
    didCompleteWithError:(NSError*)error {
    if (error != nil && !self.tooLarge && !self.malformed && !self.timedOut) {
        self.error = error;
    }
    dispatch_semaphore_signal(self.semaphore);
}

@end

namespace dusk::http {
namespace {

bool valid_redirect_url(NSURL* url) {
    if (url == nil || ![url.scheme isEqualToString:@"https"] || url.user != nil ||
        url.password != nil || url.fragment != nil || url.host.length == 0)
    {
        return false;
    }
    const NSString* absolute = url.absoluteString;
    return absolute != nil && [absolute hasPrefix:@"https://"] &&
        [absolute lengthOfBytesUsingEncoding:NSUTF8StringEncoding] <= detail::kMaxUrlBytes &&
        [absolute rangeOfString:@"\r"].location == NSNotFound &&
        [absolute rangeOfString:@"\n"].location == NSNotFound &&
        [absolute rangeOfString:@"\0"].location == NSNotFound;
}

NSInteger effective_port(NSURL* url) {
    return url.port == nil ? 443 : url.port.integerValue;
}

bool same_origin(NSURL* first, NSURL* second) {
    return first != nil && second != nil &&
        [first.scheme caseInsensitiveCompare:second.scheme] == NSOrderedSame &&
        [first.host caseInsensitiveCompare:second.host] == NSOrderedSame &&
        effective_port(first) == effective_port(second);
}

bool valid_response_headers(NSDictionary* headers) {
    if (headers.count > detail::kMaxHeaders) {
        return false;
    }
    size_t aggregate = 0;
    size_t count = 0;
    for (id key in headers) {
        const std::string name = to_string([key description]);
        const std::string value = to_string([headers[key] description]);
        if (!detail::valid_response_header({.name = name, .value = value}, count, aggregate)) {
            return false;
        }
        aggregate += name.size() + value.size();
        ++count;
    }
    return true;
}

NSString* to_nsstring(const std::string_view value) {
    return [[NSString alloc] initWithBytes:value.data()
                                    length:value.size()
                                  encoding:NSUTF8StringEncoding];
}

std::string to_string(NSString* value) {
    if (value == nil) {
        return {};
    }
    const char* utf8 = [value UTF8String];
    return utf8 == nullptr ? std::string() : std::string(utf8);
}

bool expired(const std::chrono::steady_clock::time_point deadline) {
    return std::chrono::steady_clock::now() >= deadline;
}

NSTimeInterval remaining_timeout(const std::chrono::steady_clock::time_point deadline) {
    const auto remaining = std::chrono::duration<double>(deadline - std::chrono::steady_clock::now());
    return remaining.count() <= 0.0 ? 0.0 : remaining.count();
}

dispatch_time_t timeout_deadline(const std::chrono::steady_clock::time_point deadline) {
    const auto remaining = std::chrono::duration_cast<std::chrono::nanoseconds>(
        deadline - std::chrono::steady_clock::now());
    return remaining.count() <= 0 ? DISPATCH_TIME_NOW :
        dispatch_time(DISPATCH_TIME_NOW, remaining.count());
}

bool header_name_equals(const std::string_view left, const std::string_view right) {
    if (left.size() != right.size()) {
        return false;
    }
    for (size_t index = 0; index < left.size(); ++index) {
        const unsigned char value = static_cast<unsigned char>(left[index]);
        const unsigned char expected = static_cast<unsigned char>(right[index]);
        const unsigned char folded = value >= 'A' && value <= 'Z' ? value + ('a' - 'A') : value;
        if (folded != expected) {
            return false;
        }
    }
    return true;
}

void apply_redirect_policy(Method& method, std::string& body, std::vector<Header>& headers,
    const NSInteger status, const bool crossOrigin) {
    const bool convertsPost = method == Method::Post &&
        (status == 301 || status == 302 || status == 303);
    if (convertsPost) {
        method = Method::Get;
        body.clear();
    }
    std::erase_if(headers, [&](const Header& header) {
        return (crossOrigin &&
                   (header_name_equals(header.name, "authorization") ||
                       header_name_equals(header.name, "cookie"))) ||
            (convertsPost && header.name.size() >= 8 &&
                header_name_equals(header.name.substr(0, 8), "content-"));
    });
}

NSMutableURLRequest* owned_request(NSURL* url, const Method method, const std::string& body,
    const std::vector<Header>& headers, const std::chrono::steady_clock::time_point deadline) {
    const NSTimeInterval timeout = remaining_timeout(deadline);
    if (timeout <= 0.0) {
        return nil;
    }
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = method == Method::Post ? @"POST" : @"GET";
    request.timeoutInterval = timeout;
    request.cachePolicy = NSURLRequestReloadIgnoringLocalCacheData;
    request.HTTPShouldHandleCookies = NO;
    if (method == Method::Post) {
        request.HTTPBody = [NSData dataWithBytes:body.data() length:body.size()];
    }
    for (const Header& header : headers) {
        NSString* name = to_nsstring(header.name);
        NSString* value = to_nsstring(header.value);
        if (name == nil || value == nil) {
            return nil;
        }
        [request setValue:value forHTTPHeaderField:name];
    }
    return request;
}

Error map_nsurl_error(NSError* error) {
    if (error == nil || ![error.domain isEqualToString:NSURLErrorDomain]) {
        return Error::Network;
    }
    switch (error.code) {
    case NSURLErrorTimedOut:
        return Error::Timeout;
    case NSURLErrorBadURL:
    case NSURLErrorUnsupportedURL:
        return Error::InvalidUrl;
    default:
        return Error::Network;
    }
}

}  // namespace

bool available() noexcept {
    return true;
}

Backend backend() noexcept {
    return Backend::UrlSession;
}

const char* backend_name() noexcept {
    return "NSURLSession";
}

Result request(const Request& request) {
    @autoreleasepool {
        if (!detail::valid_https_url(request.url)) {
            return {.error = Error::InvalidUrl, .message = "Invalid HTTPS URL"};
        }
        NSString* urlString = to_nsstring(request.url);
        NSURL* url = urlString == nil ? nil : [NSURL URLWithString:urlString];
        if (!valid_redirect_url(url)) {
            return {.error = Error::InvalidUrl, .message = "Failed to parse URL"};
        }
        const auto deadline = std::chrono::steady_clock::now() + request.timeout;
        DuskHttpRequestDelegate* delegate = [[DuskHttpRequestDelegate alloc] initWithRequest:request
            initialURL:url deadline:deadline];
        NSMutableURLRequest* urlRequest = [delegate initialRequest];
        if (urlRequest == nil) {
            return {.error = Error::Timeout, .message = "Request timed out"};
        }
        NSURLSessionConfiguration* configuration =
            [NSURLSessionConfiguration ephemeralSessionConfiguration];
        const NSTimeInterval timeout = request.timeout.count() / 1000.0;
        configuration.timeoutIntervalForRequest = timeout;
        configuration.timeoutIntervalForResource = timeout;
        configuration.HTTPShouldSetCookies = NO;
        NSURLSession* session = [NSURLSession sessionWithConfiguration:configuration
                                                              delegate:delegate
                                                         delegateQueue:nil];
        NSURLSessionDataTask* task = [session dataTaskWithRequest:urlRequest];
        [task resume];
        if (dispatch_semaphore_wait(delegate.semaphore, timeout_deadline(deadline)) != 0) {
            [task cancel];
            [session invalidateAndCancel];
            return {.error = Error::Timeout, .message = "Request timed out"};
        }
        [session finishTasksAndInvalidate];
        if (delegate.tooLarge) {
            return {.error = Error::TooLarge, .message = "Response body exceeded the configured limit"};
        }
        if (delegate.timedOut) {
            return {.error = Error::Timeout, .message = "Request timed out"};
        }
        if (delegate.malformed) {
            return {.error = Error::Network, .message = "Response rejected"};
        }
        if (delegate.error != nil) {
            return {.error = map_nsurl_error(delegate.error),
                .message = to_string(delegate.error.localizedDescription)};
        }
        Response response;
        if ([delegate.response isKindOfClass:[NSHTTPURLResponse class]]) {
            NSHTTPURLResponse* httpResponse = (NSHTTPURLResponse*)delegate.response;
            response.statusCode = static_cast<int>(httpResponse.statusCode);
            for (id key in httpResponse.allHeaderFields) {
                response.headers.push_back({
                    .name = to_string([key description]),
                    .value = to_string([httpResponse.allHeaderFields[key] description]),
                });
            }
        }
        if (delegate.data.length > 0) {
            response.body.assign(static_cast<const char*>(delegate.data.bytes), delegate.data.length);
        }
        return {.response = std::move(response)};
    }
}

Result get(const Request& source) {
    Request request = source;
    request.method = Method::Get;
    request.body.clear();
    return dusk::http::request(request);
}

}  // namespace dusk::http
