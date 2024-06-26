/* Joshua Holman
 * Project 3
 * Prog1.cpp
 * 24 June 2024
 */

#include <map> // for parse table
#include <iostream>
#include <string>
#include <vector> // using as stack
#include <cctype> // std::isspace
#include <iterator> // std::distance
#include <algorithm> // std::any_of

using ParseTable = std::map<char, std::map<char, std::string>>;

const ParseTable parse_table = {
  // E' = G, F'= H
  // ε = @
  {'S', {{'t', "F=E"}, {'f', "F+E"}}},
  {'E', {{'t', "F!G"}, {'f', "F!G"}}},
  {'G', {{'&', "%"}, {'%', "%TG"}, {')', "@"}, {'$', "@"}}},
  {'F', {{'t', "tH"}, {'f', "fH"}, {'=', "@"}, {'!', "@"}}},
  {'H', {{'<', "<TH"}, {'>', ">TH"}, {'=', "@"}, {'!', "@"}}},
  {'T', {{'t', "t"}, {'f', "f"}}},
};

void print_stack(const std::vector<char> &);
void push_string(std::vector<char> &, const std::string &);
void print_trace(std::string &, std::string::iterator);

int main(void) {

  std::string input;
  std::string trace = "";
  std::vector<char> stack = {};
  stack.push_back('$');
  stack.push_back('S');
  std::cout << "Enter input string: ";
  std::getline(std::cin, input);

  const std::string terminals = "tf&%<>()=!$ ";
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
  std::cout << "Trace:\n";
  print_trace(input, curr_symbol);
  int iteration = 0;


  while (!stack.empty()) {
    iteration++;
    std::cout << "\n---Currently on iteration: " << iteration << "---\n";

    // this needs to ignore whitespace to work
    while (std::isspace(*curr_symbol)) {
      curr_symbol++;
    }

    if (*curr_symbol == stack.back()) {
      std::cout << "Popping nonterm on stack top and incrementing input iterator...\n";
      trace.push_back(stack.back());
      stack.pop_back();
      curr_symbol++;
      std::cout << "Trace:\n";
      print_trace(input, curr_symbol);
      print_stack(stack);
      continue; // use continue since using if else the whole way thru is hard
      // else if top of stack is terminal 
      // (not the same terminal currently in the input buffer)
    } else if (terminals.find(stack.back()) != std::string::npos) {
      // make better error messaging
      print_stack(stack);
      std::cerr << "Non-matching terminal '" << *curr_symbol << "' found at top of stack. Terminating...\n";
      exit(1);
      
    }
    // const iterator to the column (nonterminal lhs) of parse table
    auto col = parse_table.find(stack.back());
    if (col != parse_table.end()) {
      if (col->second.find(*curr_symbol) == col->second.end()) { // if at an error entry
        std::cerr << "No table entry for [" << stack.back() << ", " << *curr_symbol <<
          "]. Terminating...\n";
        exit(2);
      }
    } else {
      std::cerr << "Invalid lhs '" << stack.back() << "' found. Terminating...\n";
      exit(2);
    }

    // table entry must exist at this point
    std::string production = parse_table.at(stack.back()).at(*curr_symbol);
    std::cout << "Using production: " << stack.back() << "->" << production << "\n";
    stack.pop_back();
    /*std::cout << "Popping production from stack...\n";*/
    /*print_stack(stack);*/
    push_string(stack, production);
    /*std::cout << "Production from table pushed to stack...\n";*/
    print_stack(stack);
    std::cout << "Trace:\n";
    print_trace(input, curr_symbol);
  }

  std::cout << "Input string " << input << " acceppted!\n";
  return 0;
}

void print_stack(const std::vector<char> &stack) {
  std::cout << "Stack: ";
  for (auto &it : stack) {
    std::cout << it;
  }
  std::cout << "\n   top--->\n";
}

void push_string(std::vector<char> &stack, const std::string &str) {
  // string const_reverse_iterator
  for (auto rit = str.crbegin(); rit != str.crend(); ++rit) {
    if (*rit != '@')
      stack.push_back(*rit);
  }
}

void print_trace(std::string &str, std::string::iterator it) {
  // print the string with a '^' indicating where the iterator currently is
  std::cout << str << "\n";

  auto pos = std::distance(str.begin(), it);

  std::cout << std::string(pos, ' ') << "^\n";
}
