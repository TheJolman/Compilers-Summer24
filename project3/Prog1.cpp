/* Joshua Holman
 * Project 3
 * Prog1.cpp
 * 24 June 2024
 */

#include <map>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cctype> // std::cctype
#include <algorithm>

const std::map<char, std::map<char, std::string>> parse_table = {
  // E' = G, F'= H
  // ε = 
  {'S', {{'t', "F=E"}, {'f', "F+E"}}},
  {'E', {{'t', "F!G"}, {'f', "F!G"}}},
  {'G', {{'&', "%"}, {'%', "%TG"}, {')', "@"}, {'$', "@"}}},
  {'F', {{'t', "tH"}, {'f', "fH"}, {'=', "@"}, {'!', "@"}}},
  {'H', {{'<', "<TH"}, {'>', ">TH"}, {'=', "@"}, {'!', "@"}}},
  {'T', {{'t', "T"}, {'f', "F"}}},
};

void print_stack(const std::stack<char> &);
void push_string(std::stack<char> &, const std::string &);

int main() {

  std::string input;
  std::stack<char> stack;
  stack.emplace('$');
  stack.emplace('S');
  std::cout << "Enter input string: ";
  std::getline(std::cin, input);

  const std::string terminals = "tf&%<>()=!$ ";
  // first check has any invalid nonterms
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
  auto curr_symbol = input.cbegin(); 
  std::cout << "First symbol: " << *curr_symbol << "\n";
  int iteration = 0;


  while (!stack.empty()) {
    iteration++;
    std::cout << "\n---Currently on iteration: " << iteration << "---\n";

    // this needs to ignore whitespace to work
    while (std::isspace(*curr_symbol)) {
      curr_symbol++;
    }

    if (*curr_symbol == stack.top()) {
      std::cout << "Popping stack and incrementing input iterator...\n";
      stack.pop();
      print_stack(stack);
      curr_symbol++;
      continue;
      // else if top of stack is terminal 
      // (not the same terminal currently in the input buffer)
    } else if (terminals.find(stack.top()) != std::string::npos) {
      // make better error messaging
      print_stack(stack);
      std::cerr << "Non-matching terminal '" << *curr_symbol << "' found at top of stack. Terminating...\n";
      exit(1);
      
    }
    // const iterator to the column (nonterminal lhs) of parse table
    auto col = parse_table.find(stack.top());
    if (col != parse_table.end()) {
      if (col->second.find(*curr_symbol) == col->second.end()) { // if at an error entry
        std::cerr << "Error condition in parse table accessed. Terminating...\n";
        exit(2);
      }
    } else {
      std::cerr << "Invalid lhs '" << stack.top() << "' found. Terminating...\n";
      exit(2);
    }

    // table entry must exist at this point
    std::string production = parse_table.at(stack.top()).at(*curr_symbol);
    std::cout << "Using production: " << stack.top() << "->" << production << "\n";
    stack.pop();
    std::cout << "Popping production from stack...\n";
    print_stack(stack);
    push_string(stack, production);
    std::cout << "Production from table pushed to stack...\n";
    print_stack(stack);


  }

  std::cout << "Input string " << input << " acceppted!\n";
  return 0;
}

void print_stack(const std::stack<char> &stack) {
  /*auto stack_copy = std::stack(stack);*/
  std::stack<char> stack_copy = stack;
  std::vector<char> vec = {};
  std::cout << "Stack: ";
  while (!stack_copy.empty()) {
    std::cout << stack_copy.top();
    stack_copy.pop();
  }
  std::cout << "\n       ^---top\n";
}

void push_string(std::stack<char> &stack, const std::string &str) {
  // string const_reverse_iterator
  for (auto rit = str.crbegin(); rit != str.crend(); ++rit) {
    if (*rit != '@')
      stack.push(*rit);
  }
}
