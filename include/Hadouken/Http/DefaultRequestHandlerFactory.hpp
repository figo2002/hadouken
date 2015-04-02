#ifndef HADOUKEN_HTTP_DEFAULTREQUESTHANDLERFACTORY_HPP
#define HADOUKEN_HTTP_DEFAULTREQUESTHANDLERFACTORY_HPP

#include <map>
#include <memory>
#include <string>

#include <Hadouken/Http/JsonRpc/RpcMethod.hpp>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Util/AbstractConfiguration.h>

namespace Hadouken
{
    namespace Http
    {
        using namespace Hadouken::Http::JsonRpc;

        class WebSocketConnectionManager;

        class DefaultRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
        {
        public:
            DefaultRequestHandlerFactory(const Poco::Util::AbstractConfiguration& config);

            Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

        private:
            const Poco::Util::AbstractConfiguration& config_;
            std::string virtualPath_;
            std::map<std::string, std::shared_ptr<RpcMethod>> methods_;
            std::unique_ptr<WebSocketConnectionManager> wsConnectionManager_;
        };
    }
}

#endif