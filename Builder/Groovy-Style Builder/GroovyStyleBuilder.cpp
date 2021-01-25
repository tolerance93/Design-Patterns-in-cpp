#include <string>
#include <vector>
#include <iostream>

/**
 * Initialization list를 활용하여 DSL(Domain Specific Language)을 생성하기
 */
namespace html {
  struct Tag
  {
    std::string name;
    std::string text;
    std::vector<Tag> children;
    std::vector<std::pair<std::string, std::string>> attributes;

    friend std::ostream& operator<<(std::ostream& os, const Tag& tag)
    {
      os << "<" << tag.name;

      for (const auto& att : tag.attributes)
        os << " " << att.first << "=\"" << att.second << "\"";

      if (tag.children.size() == 0 && tag.text.length() == 0)
      {
        os << "/>" << std::endl;
      }
      else
      {
        os << ">" << std::endl;

        if (tag.text.length())
          os << tag.text << std::endl;

        for (const auto& child : tag.children)
          os << child;

        os << "</" << tag.name << ">" << std::endl;
      }
      return os;
    }
      /**
       * 여기서 사용자가 생성자를 이용해 직접적으로 인스턴스화하는 것은 바람직하지 않다!
       */
  protected:

    Tag(const std::string& name, const std::string& text)
      : name{name},
        text{text}
    {
    }


    Tag(const std::string& name, const std::vector<Tag>& children)
      : name{name},
        children{children}
    {
    }
  };

/**
 * [Initializier_list<T>]
 * children이 vector<Tag>가 아니라 단순 Tag인데도 vector로 변환되어 생성자에 전해진다..!
 * vector defines a constructor which takes initializer_list<T> as argument,
 * so that it could initialize itself with the elements in the initializer-list.
 */
  struct P : Tag
  {
    explicit P(const std::string& text)
      : Tag{"p", text}
    {
    }

    P(std::initializer_list<Tag> children)
      : Tag("p", children)
    {
    }
    
  };

  struct IMG : Tag
  {
    explicit IMG(const std::string& url)
      : Tag{"img", ""}
    {
      attributes.emplace_back(make_pair("src", url));
    }
  };
}
/**
 * [Uniform initialzation]
 * It introduces brace-initialization that uses braces ({}) to enclose initializer values. The syntax is as follows:
 * type var_name{arg1, arg2, ....arg n}
 */
int main()
{
  using namespace html;

  std::cout <<

    P {
      IMG {"http://pokemon.com/pikachu.png"}
    }

    << std::endl;

  getchar();
  return 0;
}
