#include "stdafx.h"

struct Pingable
{
  virtual wstring ping(const wstring& message) = 0;
};

/**
 * local
 * 매 응답마다 새로운 문자열을 생성: 웹서비스의 동작 방식을 따라한 것
 */
struct Pong : Pingable
{
  wstring ping(const wstring& message) override
  {
    return message + L" pong";
  }
};

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

/**
 * 커뮤니케이션 프록시. 객체의 물리적 위치를 바꾸면서도 API를 거의 동일하게 유지할 수 있게 해줌.
 * 프록시로 구현된 API들은 REST API와 같은 원격 서비스의 투영일 뿐이다.
 */
struct RemotePong : Pingable
{
  wstring ping(const wstring& message) override
  {
    wstring result;
    http_client client(U("http://localhost:64959/"));
    uri_builder builder(U("/api/values/"));
    builder.append(message);
    auto task = client.request(methods::GET, builder.to_string())
      .then([=](http_response r)
    {
      return r.extract_string();
    });
    task.wait();
    return task.get();
  }
};

void tryit(Pingable& pp)
{
  wcout << pp.ping(L"ping") << "\n";
}

int main()
{
  RemotePong pp;
  for (size_t i = 0; i < 3; i++)
  {
    tryit(pp);
  }

  return 0;
}

