/* Joshua Holman
 * Project 3
 * Prog2.cpp
 * 24 June 2024
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype> // std::cctype
#include <cassert>

using StateTable = std::map<std::string, std::map<std::string, std::string>>;
using VecStack = std::vector<std::string>;
using Production = std::pair<std::string, std::string>;
using Grammar = std::vector<Production>;

// this might be more readable than how I did it in Prog1
StateTable create_state_table() {
  StateTable table;
  table["0"] = {{"i", "S5"}, {"(", "S4"}, {"E", "1"}, {"T", "2"}, {"F", "3"}};

  table["1"] = {{"+", "S6"}, {"-", "S7"}, {"$", "acc"}};

  table["2"] = {{"+", "R3"}, {"-", "R3"}, {"*", "S8"}, {"/", "S9"}, {")", "R3"}, {"$", "R3"}};

  table["3"] = {{"+", "R6"}, {"-", "R6"}, {"*", "R6"}, {"/", "R6"}, {")", "R6"}, {"$", "R6"}};

  table["4"] = {{"i", "S5"}, {"(", "S4"}, {"E", "10"}, {"T", "2"}, {"F", "3"}};

  table["5"] = {{"+", "R8"}, {"-", "R8"}, {"*", "R8"}, {"/", "R8"}, {")", "R8"}, {"$", "R8"}};

  table["6"] = {{"i", "S5"}, {"(", "S4"}, {"T", "11"}, {"F", "3"}};

  table["7"] = {{"i", "S5"}, {"(", "S4"}, {"T", "12"}, {"F", "3"}};

  table["8"] = {{"i", "S5"}, {"(", "S4"}, {"F", "13"}};

  table["9"] = {{"i", "S5"}, {"(", "S4"}, {"F", "14"}};

  table["10"] = {{"+", "S6"}, {"-", "S7"}, {")", "S15"}};

  table["11"] = {{"+", "R1"}, {"-", "R1"}, {"*", "S8"}, {"/", "S9"}, {")", "R1"}, {"$", "R1"}};

  table["12"] = {{"+", "R2"}, {"-", "R2"}, {"*", "S8"}, {"/", "S9"}, {")", "R2"}, {"$", "R2"}};

  table["13"] = {{"+", "R4"}, {"-", "R4"}, {"*", "R4"}, {"/", "R4"}, {")", "R4"}, {"$", "R4"}};

  table["14"] = {{"+", "R5"}, {"-", "R5"}, {"*", "R5"}, {"/", "R5"}, {")", "R5"}, {"$", "R5"}};

  table["15"] = {{"+", "R7"}, {"-", "R7"}, {"*", "R7"}, {"/", "R7"}, {")", "R7"}, {"$", "R7"}};
  
  return table;
}

Grammar create_grammar() {
  Grammar grammar;
  grammar.push_back(std::make_pair("E", "E+T"));
  grammar.push_back(std::make_pair("E", "E-T"));
  grammar.push_back(std::make_pair("E", "T"));
  grammar.push_back(std::make_pair("T", "T*F"));
  grammar.push_back(std::make_pair("T", "T/F"));
  grammar.push_back(std::make_pair("T", "F"));
  grammar.push_back(std::make_pair("F", "(E)"));
  grammar.push_back(std::make_pair("F", "i"));
  return grammar;
}

void print_state_table(const StateTable &);
void print_stack(const VecStack &);
void push_string(std::vector<char> &, const std::string &);
void print_trace(std::string &, std::string::iterator);
std::string get_table_action(const StateTable &, const std::string &, const std::string &);
void reduce(const StateTable &, VecStack &, const Production &);


int main(void) {
  StateTable state_table = create_state_table();
  Grammar grammar = create_grammar();
  /*print_state_table(state_table);*/

  // initial important vars
  std::string input;
  std::string trace = "";
  VecStack stack = {};

  stack.push_back("0");
  std::cout << "Enter input string: ";
  std::getline(std::cin, input);

  const std::string terminals = "i+-*/()$ ";
  /* First check has any invalid nonterms.
   * This if statements uses std::any_of with a lambda function to check if any characters 
   * from the input string aren't in the list of allowed terminals */
  if (std::any_of(input.begin(), input.end(), 
        [&terminals](char ch) {
        return terminals.find(ch) == std::string::npos; })) {
    std::cerr << "input string contains one or more invalid characters. Terminating...\n";
    exit(1);
  }


  if (input.back() != '$')
    input.push_back('$');
  std::cout << "Input String: " << input << "\n";
  print_stack(stack);

  // const iterator to string
  auto curr_symbol = input.begin(); 
  print_trace(input, curr_symbol);

  // vars used in loop
  int iteration = 0;
  std::string state;
  std::string incoming_token;
  std::string table_action;
  int rule_num;
  std::string new_state;
  Production production;

  try {

    while (!stack.empty()) {
      iteration++;
      std::cout << "\n---Currently on iteration: " << iteration << "---\n";
      print_stack(stack);
      print_trace(input, curr_symbol);

      // this needs to ignore whitespace to work
      while (std::isspace(*curr_symbol)) {
        curr_symbol++;
      }

      state = stack.back();
      incoming_token = *curr_symbol;
      table_action = get_table_action(state_table, state, incoming_token);

      switch (table_action.front()) {
        case 'S':
          /*std::cout << "in case 'S'\n";*/
          // shift
          stack.push_back(incoming_token);
          new_state = table_action.substr(1);
          stack.push_back(new_state); // append state num 
          curr_symbol++;
          std::cout << "Pushed " << incoming_token << " and " << new_state << " to stack.\n";
          break;
        case 'R':
          /*std::cout << "in case 'R'\n";*/
          // reduce
          /*std::cout << "before converting rule_num to int\n";*/
          rule_num = std::stoi(table_action.substr(1)) - 1;
          /*std::cout << "after converting rule_num to int\n";*/
          if (grammar.size() > rule_num) {
            production = grammar[rule_num];
          } else {
            std::cerr << "rule_num out of bounds.\n";
            exit(1);
          }
          std::cout << "Using rule " << rule_num << " which is " << production.first <<
            "->" << production.second << "\n";
          reduce(state_table, stack, production);
          break;
        case 'a':
          // accept state
          std::cout << "String accepted!\n";
          exit(0);
        default: 
          // error msg
          std::cout << "Error: something went wrong bruv.\n";
          print_stack(stack);
          print_trace(input, curr_symbol);
          exit(1); 
      }
    }

  } catch (const std::bad_alloc& e) {
    std::cerr << "mem allocation failed: " << e.what() << "\n";
    std::cerr << "stack size: " << stack.size() << "\n";
    print_trace(input, curr_symbol);
  }

  return 0;
}


/* Functions definitions */

void print_state_table(const StateTable &table) {
  // structured bindings :O
  for (const auto& [state, transitions] : table) {
    std::cout << "State " << state << ":\n";
    for (const auto& [symbol, action] : transitions) {
      std::cout << "  " << symbol << " -> " << action << "\n";
    }
    std::cout << "\n";
  }
}

void print_stack(const VecStack &stack) {
  std::cout << "Stack: ";
  for (auto &it : stack) {
    std::cout << it << " ";
  }
  /*std::cout << "\n   top--->\n";*/
  std::cout << "\n";
}

void push_string(std::vector<char> &stack, const std::string &str) {
  std::cout << "in push_string\n";
  // string const_reverse_iterator
  for (auto rit = str.crbegin(); rit != str.crend(); ++rit) {
    stack.push_back(*rit);
  }
}

void print_trace(std::string &str, std::string::iterator it) {
  // print the string with a '^' indicating where the iterator currently is
  std::string trace_literal = "Trace: ";
  std::cout << trace_literal;
  std::cout << str << "\n";

  auto pos = std::distance(str.begin(), it);

  std::cout << std::string(pos + trace_literal.length(), ' ') << "^\n";
}

std::string get_table_action(const StateTable &table, const std::string &state, const std::string &symbol) {
  /*std::cout << "in get_table_action\n";*/
  auto state_it = table.find(state);
  if (state_it != table.end()) {
    auto symbol_it = state_it->second.find(symbol);
    if (symbol_it != state_it->second.end()) {
      /*std::cout << "about to return...\n";*/
      return symbol_it->second;
    }
  }
  std::cout << "Error: No table entry for [" << state << ", " << symbol << "]\n";
  exit(1);
  /*return "Error";*/
}

void reduce(const StateTable &table, VecStack &stack, const Production &production) {
  /* If the RHS contains k symbols, pop the top k state-symbol pairs off the stack
   * Note the state on top of the stack
   * Enter go-to part of the table at [stack.back(), lhs], then push lhs and 
   * the entry found in the table to the stack */
  std::cout << "in reduce\n";
  auto [lhs, rhs] = production;
  size_t elements_to_erase = 2 * rhs.length();
  stack.erase(stack.end() - elements_to_erase, stack.end());
  std::string table_entry = get_table_action(table, stack.back(), lhs);
  std::cout << "pushing: " << lhs << "\n";
  stack.push_back(lhs);
  std::cout << "pushing: " << table_entry << "\n";
  stack.push_back(table_entry);

}
