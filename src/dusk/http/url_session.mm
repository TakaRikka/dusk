#include "http.hpp"

#import <Foundation/Foundation.h>

#include <algorithm>
#include <string_view>
#include <utility>

@interface DuskHttpRequestDelegate : NSObject <NSURLSessionDataDelegate, NSURLSessionTaskDelegate>
@property(nonatomic) dispatch_semaphore_t semaphore;
@property(nonatomic) size_t maxBodyBytes;
@property(nonatomic, strong) NSMutableData* data;
@property(nonatomic, strong) NSURLResponse* response;
@property(nonatomic, strong) NSError* error;
@property(nonatomic) BOOL tooLarge;
- (instancetype)initWithMaxBodyBytes:(size_t)maxBodyBytes;
@end

@implementation DuskHttpRequestDelegate

- (instancetype)initWithMaxBodyBytes:(size_t)maxBodyBytes {
    self = [super init];
    if (self != nil) {
        _semaphore = dispatch_semaphore_create(0);
        _maxBodyBytes = maxBodyBytes;
        _data = [NSMutableData data];
    }
    return self;
}

- (void)URLSession:(NSURLSession*)session
              task:(NSURLSessionTask*)task
    willPerformHTTPRedirection:(NSHTTPURLResponse*)response
                    newRequest:(NSURLRequest*)request
              completionHandler:(void (^)(NSURLRequest*))completionHandler {
    if ([[request.URL.scheme lowercaseString] isEqualToString:@"https"]) {
        completionHandler(request);
    } else {
        completionHandler(nil);
    }
}

- (void)URLSession:(NSURLSession*)session
          dataTask:(NSURLSessionDataTask*)dataTask
didReceiveResponse:(NSURLResponse*)response
 completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler {
    self.response = response;
    completionHandler(NSURLSessionResponseAllow);
}

- (void)URLSession:(NSURLSession*)session
          dataTask:(NSURLSessionDataTask*)dataTask
    didReceiveData:(NSData*)data {
    if (data.length > self.maxBodyBytes ||
        self.data.length > self.maxBodyBytes - data.length) {
        self.tooLarge = YES;
        [dataTask cancel];
        return;
    }
    [self.data appendData:data];
}

- (void)URLSession:(NSURLSession*)session
                    task:(NSURLSessionTask*)task
    didCompleteWithError:(NSError*)error {
    if (error != nil && !self.tooLarge) {
        self.error = error;
    }
    dispatch_semaphore_signal(self.semaphore);
}

@end

namespace dusk::http {
namespace {

NSString* to_nsstring(std::string_view value) {
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

dispatch_time_t timeout_deadline(std::chrono::milliseconds timeout) {
    const auto milliseconds = std::max<std::chrono::milliseconds::rep>(1, timeout.count());
    return dispatch_time(DISPATCH_TIME_NOW,
        static_cast<int64_t>(milliseconds) * static_cast<int64_t>(NSEC_PER_MSEC));
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

Result get(const Request& request) {
    @autoreleasepool {
        if (request.url.empty()) {
            return {
                .error = Error::InvalidUrl,
                .message = "URL is empty",
            };
        }
        if (!request.url.starts_with("https://")) {
            return {
                .error = Error::UnsupportedScheme,
                .message = "Only https:// URLs are supported",
            };
        }

        NSString* urlString = to_nsstring(request.url);
        if (urlString == nil) {
            return {
                .error = Error::InvalidUrl,
                .message = "URL is not valid UTF-8",
            };
        }

        NSURL* url = [NSURL URLWithString:urlString];
        if (url == nil || ![[url.scheme lowercaseString] isEqualToString:@"https"]) {
            return {
                .error = Error::InvalidUrl,
                .message = "Failed to parse URL",
            };
        }

        NSMutableURLRequest* urlRequest = [NSMutableURLRequest requestWithURL:url];
        urlRequest.HTTPMethod = @"GET";
        urlRequest.timeoutInterval = request.timeout.count() / 1000.0;
        urlRequest.cachePolicy = NSURLRequestReloadIgnoringLocalCacheData;
        for (const Header& header : request.headers) {
            NSString* name = to_nsstring(header.name);
            NSString* value = to_nsstring(header.value);
            if (name == nil || value == nil) {
                return {
                    .error = Error::InvalidUrl,
                    .message = "Request header is not valid UTF-8",
                };
            }
            [urlRequest setValue:value forHTTPHeaderField:name];
        }

        NSURLSessionConfiguration* configuration =
            [NSURLSessionConfiguration ephemeralSessionConfiguration];
        configuration.timeoutIntervalForRequest = request.timeout.count() / 1000.0;
        configuration.timeoutIntervalForResource = request.timeout.count() / 1000.0;

        DuskHttpRequestDelegate* delegate =
            [[DuskHttpRequestDelegate alloc] initWithMaxBodyBytes:request.maxBodyBytes];
        NSURLSession* session = [NSURLSession sessionWithConfiguration:configuration
                                                              delegate:delegate
                                                         delegateQueue:nil];
        NSURLSessionDataTask* task = [session dataTaskWithRequest:urlRequest];
        [task resume];

        if (dispatch_semaphore_wait(delegate.semaphore, timeout_deadline(request.timeout)) != 0) {
            [task cancel];
            [session invalidateAndCancel];
            return {
                .error = Error::Timeout,
                .message = "Request timed out",
            };
        }

        [session finishTasksAndInvalidate];

        Response response;
        if ([delegate.response isKindOfClass:[NSHTTPURLResponse class]]) {
            NSHTTPURLResponse* httpResponse = (NSHTTPURLResponse*)delegate.response;
            response.statusCode = static_cast<int>(httpResponse.statusCode);
            NSDictionary* headers = httpResponse.allHeaderFields;
            for (id key in headers) {
                id value = headers[key];
                response.headers.push_back({
                    .name = to_string([key description]),
                    .value = to_string([value description]),
                });
            }
        }
        if (delegate.data != nil && delegate.data.length > 0) {
            response.body.assign(static_cast<const char*>(delegate.data.bytes),
                static_cast<size_t>(delegate.data.length));
        }

        if (delegate.tooLarge) {
            return {
                .error = Error::TooLarge,
                .message = "Response body exceeded the configured limit",
                .response = std::move(response),
            };
        }
        if (delegate.error != nil) {
            return {
                .error = map_nsurl_error(delegate.error),
                .message = to_string(delegate.error.localizedDescription),
                .response = std::move(response),
            };
        }

        return {
            .response = std::move(response),
        };
    }
}

}  // namespace dusk::http
