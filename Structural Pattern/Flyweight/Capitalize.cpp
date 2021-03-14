#include <iostream>
#include <string>
#include <ostream>
#include <vector>
/**
 * 문자열 범위 기능 예제
 */
using namespace std;

/**
 * Brute Force: 원본 텍스트와 같은 크기의 이진 배열을 만들어 대문자인지 표시
 * 텍스트의 시작/끝 표시만으로 요구 조건을 나타내기에 충분함에도 불구하고, 텍스트의 모든 문자마다 이진 플래그를 만든다는 것은 매우 큰 낭비.
 */
class FormattedText
{
  string plain_text;
  bool *caps;
public:
  explicit FormattedText(const string& plainText)
    : plain_text{plainText}
  {
    caps = new bool[plainText.length()];
  }
  ~FormattedText()
  {
    delete[] caps;
  }
  void capitalize(int start, int end)
  {
    for (int i = start; i <= end; ++i)
      caps[i] = true;
  }
  
  friend std::ostream& operator<<(std::ostream& os, const FormattedText& obj)
  {
    string s;
    for (int i = 0; i < obj.plain_text.length(); ++i)
    {
      char c = obj.plain_text[i];
      s += (obj.caps[i] ? toupper(c) : c);
    }
    return os << s;
  }
};

class BetterFormattedText
{
public:
    //플라이웨이트
  struct TextRange
  {
    int start, end;
    bool capitalize, bold, italic;

      /**
       * 주어진 위치의 문자가 포맷의 적용을 받을 필요가 있는가?
       */
    bool covers(int position) const
    {
      return position >= start && position <= end;
    }
  };

    //메모리 절약을 위해서는 중복범위 체크도 있어야 한다.
  TextRange& get_range(int start, int end)
  {
    formatting.emplace_back(TextRange{ start, end });
      //마지막 항목에 대한 참조 리턴
    return *formatting.rbegin();
  }

  explicit BetterFormattedText(const string& plainText)
    : plain_text{plainText}
  {
  }

  friend std::ostream& operator<<(std::ostream& os, const BetterFormattedText& obj)
  {
    string s;
    for (size_t i = 0; i < obj.plain_text.length(); i++)
    {
      auto c = obj.plain_text[i];
      for (const auto& rng : obj.formatting)
      {
        if (rng.covers(i) && rng.capitalize)
          c = toupper(c);
        s += c;
      }
    }
    return os << s;
  }

private:
  string plain_text;
  vector<TextRange> formatting;
};

int main(int argc, char* argv[])
{
  FormattedText ft("This is a brave new world");
  ft.capitalize(10, 15);
  cout << ft << endl;

  BetterFormattedText bft("This is a brave new world");
  bft.get_range(10, 15).capitalize = true;
  cout << bft << endl;
}
