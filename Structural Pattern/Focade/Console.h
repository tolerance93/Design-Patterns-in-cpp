#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Window.h"
#include "TextBuffer.h"
using namespace std;

/**
 * Focade 디자인 패턴: 하나 이상의 복잡한 서브 시스템 앞에 단순한 인터페이스를 두기 위한 방법.
 * 다수의 버퍼와 다수의 뷰포트가 연동되는 복잡한 콘솔이든, 단일 버퍼에 단일 뷰포트만 존재하는 단순한 콘솔이든,
 * 편리하고 직관적인  API를 통해 이용할 수 있게 함.
 */

class Console
{
public:
  static Console& instance()
  {
    static Console console;
    return console;
  }

  vector<shared_ptr<Window>> windows;

  // cannot make this static
  shared_ptr<Window> single(const string& title, uint8_t charsWide, uint8_t charsHigh)
  {
    auto w = make_shared<Window>(title, charsWide * charWidth, charsHigh * charHeight);
    auto buffer = make_shared<TextBuffer>(charsWide, charsHigh);
    w->buffers.push_back(buffer);
    auto viewport = make_shared<Viewport>(buffer);
    w->viewports.push_back(viewport);
    windows.push_back(w);
    return w;
  }

  shared_ptr<Window> multicolumn(const string& title, uint8_t columnCount, uint8_t columnWidth, uint8_t charsHigh)
  {
    auto w = make_shared<Window>(title, columnCount * columnWidth * charWidth, charsHigh * charHeight);
    for (uint8_t c = 0; c < columnCount; ++c)
    {
      auto buffer = make_shared<TextBuffer>(columnWidth, charsHigh);
      w->buffers.push_back(buffer);
      auto viewport = make_shared<Viewport>(buffer, buffer->get_size(), Point{}, Point{ columnWidth*c, 0 });
      w->viewports.push_back(viewport);
      windows.push_back(w);
    }
    return w;
  }

private:
  const int charWidth = 10, charHeight = 14;
  Console(){}
  Console(Console const&) = delete;
  void operator=(Console const&) = delete;
};
