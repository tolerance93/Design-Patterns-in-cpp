#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Sentence
{
  struct WordToken
  {
    bool capitalize;
  };

  Sentence(const string& text)
  {
    // todo
      string s;
      for (int i = 0; i < text.size(); i++)
      {
          if (text[i] == ' ')
          {
              sentences.emplace_back(s);
              s = "";
          }
          else
          {
              s += text[i];
          }
      }
      sentences.emplace_back(s);
      capital = vector<WordToken>(sentences.size());
  }

  WordToken& operator[](size_t index)
  {
    // todo
      capital[index].capitalize = true;
      return capital[index];
  }

  string str() const
  {
    // todo
      string s;
      for (int i = 0; i < sentences.size(); i++)
      {
          if (capital[i].capitalize)
          {
              for (auto& c : sentences[i])
              {
                  s += toupper(c);
              }
          }
          else
          {
              s += sentences[i];
          }
          if (i + 1 < sentences.size()) s += ' ';
      }
      return s;
  }
    

    vector<string> sentences;
    vector<WordToken> capital;
};

int main()
{
    Sentence sentence("alhpa beta gamma");
    sentence[1].capitalize = true;
    cout << sentence.str();
}
