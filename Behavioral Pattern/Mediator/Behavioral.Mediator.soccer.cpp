/**
 * 매개자 디자인 패턴은 시스템 내 컴포넌트 모두가 참조할 수 있는 어떤 중간자를 컴포넌트 간에 서로 직접적으로 참조하지 않더라도
 * 커뮤니케이션을 할 수 있게 한다는 것을 기본 아이디어로 한다.
 */

#include <iostream>
#include <string>
#include <vector>
struct Game;
using namespace std;
// 이벤트 기능을 위해
#include <boost/signals2.hpp>
using namespace boost::signals2;

struct EventData
{
  virtual ~EventData() = default;
  virtual void print() const = 0;
};

struct Player;
struct PlayerScoredData : EventData
{
  string player_name;
  int goals_scored_so_far;

  PlayerScoredData(const string& player_name, const int goals_scored_so_far)
    : player_name(player_name),
      goals_scored_so_far(goals_scored_so_far)
  {
  }

  void print() const override
  {
    cout << player_name << " has scored! (their " 
      << goals_scored_so_far << " goal)" << "\n";
  }
};

/**
 * 매개자. 이벤트 기반 구조에서는 매개자가 직접 일을 수행하지 않는다
 */
struct Game
{
  signal<void(EventData*)> events; // observer
};

struct Player
{
  string name;
  int goals_scored = 0;
  Game& game;


  Player(const string& name, Game& game)
    : name(name),
      game(game)
  {
  }

  void score()
  {
    goals_scored++;
    PlayerScoredData ps{name, goals_scored};
      //모든 이벤트의 수신처로 등록된 객체들에 알림을 전송한다.
    game.events(&ps);
  }
};

struct Coach
{
  Game& game;

  explicit Coach(Game& game)
    : game(game)
  {
    // celebrate if player has scored <3 goals
      // 생성자에서 game.events에 수신 등록
    game.events.connect([](EventData* e)
    {
      PlayerScoredData* ps = dynamic_cast<PlayerScoredData*>(e);
      if (ps && ps->goals_scored_so_far < 3)
      {
        cout << "coach says: well done, " << ps->player_name << "\n";
      }
    });
  }
};

int main()
{
  Game game;
  Player player{ "Sam", game };
  Coach coach{ game };

  player.score();
  player.score();
  player.score(); // ignored by coach

  getchar();
  return 0;
}
