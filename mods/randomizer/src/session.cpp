#include "session.hpp"

namespace randomizer::session {
ServiceManager svc_mng;

ModResult initialize(const ServiceManager& services) {
    svc_mng = services;

    return MOD_OK;
}


}