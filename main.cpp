#include "random.h"
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace PlayerGold {
constexpr int maxGold{120};
constexpr int minGold{80};

} // namespace PlayerGold
namespace Potion {

enum Type { healing, mana, speed, invisibility, maxPotionLength };

constexpr std::array types{healing, mana, speed, invisibility};

constexpr std::array potionCost{20, 30, 12, 50};
constexpr std::array<std::string_view, maxPotionLength> potionName{
    "healing", "mana", "speed", "invisibility"};

static_assert(std::size(types) == maxPotionLength);
static_assert(std::size(potionCost) == maxPotionLength);
static_assert(std::size(potionName) == maxPotionLength);

constexpr int minPotionVal() {
  int min{Potion::potionCost[0]};
  for (std::size_t i{0}; i < Potion::potionCost.size(); ++i) {
    if (Potion::potionCost[i] < min) {
      min = Potion::potionCost[i];
    }
  }
  return min;
}

constexpr int minPotionValue{minPotionVal()};

} // namespace Potion

class Player {
public:
  void UpdateInventory(int idx) {
    int cost{Potion::potionCost[idx]};
    if (m_gold < cost) {
      std::cout << "You can't afford that.\n";
      return;
    }
    std::string_view potion{Potion::potionName[idx]};
    m_potionInventory.push_back(potion);
    m_gold -= cost;

    std::cout << "Thanks! You bought " << Potion::potionName[idx]
              << " and now have " << m_gold << " gold. \n";
  }
  int getPlayerGold() const { return m_gold; }
  bool getIsPlaying() const { return m_isPlaying; }
  const std::vector<std::string_view>& getIventory() const {
    return m_potionInventory;
  }
  void setNotPlaying() {
    m_isPlaying = false;
    return;
  }

  std::string getPlayerName() {
    while (true) {

      std::string name{};
      std::cout << "Enter Your name: ";

      std::getline(std::cin >> std::ws, name);

      if (!std::cin) {
        std::cout << "Please enter your name";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
      return name;
    }
  }
  void playerGreet() {
    std::cout << "Hello " << m_name << " you have " << m_gold << " gold.\n\n";
  }
  void printInventory() {
    std::cout << "Your items: \n";
    std::cout << "*******************\n";

    std::vector<std::string_view> inven{getIventory()};
    int num{1};
    for (const auto& item : inven) {
      std::cout << num << ") " << item << '\n';
      ++num;
    }
  }

private:
  bool m_isPlaying{true};
  std::string m_name{getPlayerName()};
  std::vector<std::string_view> m_potionInventory{
      std::vector<std::string_view>{}};
  int m_gold{Random::get(PlayerGold::minGold, PlayerGold::maxGold)};
};
void shop() {
  std::cout << "Here is out selection for today:\n";

  for (auto potion : Potion::types) {
    std::cout << potion << ") " << Potion::potionName[potion] << " costs "
              << Potion::potionCost[potion] << '\n';
  }
}
void ignoreLine() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
bool hasUnextractedInput() {
  return !std::cin.eof() && std::cin.peek() != '\n';
}
int charToInt(char c) { return c - '0'; }

int handlePotionSelection() {
  while (true) {
    std::cout << "Enter the number of the potion you'd like to buy, or 'q' "
                 "to quit: ";

    char selection{};
    std::cin >> selection;
    if (selection == 'q') {
      return -1;
    }
    int intSelection{charToInt(selection)};
    if (!std::cin) {
      std::cin.clear();
      ignoreLine();
      continue;
    }
    if (hasUnextractedInput()) {
      std::cin.clear();
      ignoreLine();
      continue;
    }
    if (intSelection < 0 || intSelection > Potion::maxPotionLength - 1) {
      std::cout << "Please choose between 0 and " << Potion::maxPotionLength - 1
                << '\n';
      continue;
    }
    return intSelection;
  }
}

int main() {

  std::cout << "Welcome to Roscoe's potion emporium!\n";
  Player p{};
  p.playerGreet();
  shop();
  do {
    int potionSelect{handlePotionSelection()};
    if (potionSelect == -1) {
      p.setNotPlaying();
    } else {
      p.UpdateInventory(potionSelect);
    }

  } while (p.getIsPlaying() && p.getPlayerGold() >= Potion::minPotionValue);
  if (!p.getIsPlaying()) {
    std::cout << "Come back if you need something!";
    p.printInventory();
    return 0;
  } else {
    std::cout << "\nYou can't afford anything else. Thanks for shopping at "
                 "Roscoe's potion emporium!\n";
    p.printInventory();
    return 0;
  }
}
