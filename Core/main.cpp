#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include "MutatorManager.h"
#include "json.hpp"

using json = nlohmann::ordered_json;

namespace opt {
using namespace llvm;

cl::OptionCategory mutOpts("options for YSMut");

cl::opt<std::string> mutationRange("mutation-range",
    cl::desc("specify the range to mutate"),
    cl::cat(mutOpts));

cl::opt<std::string> mutator("mutator",
    cl::desc("Specify the mutator to apply"),
    cl::cat(mutOpts));

cl::opt<std::string> output("o",
    cl::desc("Specify the output file"), cl::cat(mutOpts));
cl::opt<std::string> input("i",
    cl::desc("Specify the input file"), cl::cat(mutOpts));

cl::opt<unsigned> seed("seed", cl::init(0),
    cl::desc("Specify the random seed"), cl::cat(mutOpts));

cl::opt<bool> listMutators("list-mutators",
    cl::desc("List all mutators"), cl::cat(mutOpts));

cl::opt<std::string> tryMutators("try-mutators",
    cl::desc(
        "Try all specific mutators in specified order"),
    cl::cat(mutOpts));

cl::opt<bool> randomlyTryAllMutators(
    "randomly-try-all-mutators", cl::init(false),
    cl::desc(
        "Try all specific mutators in specified order"),
    cl::cat(mutOpts));

} // namespace opt

std::vector<std::string> splitString(
    const std::string &str, char delimiter) {
  std::stringstream ss(str);
  std::string token;
  std::vector<std::string> slices;
  while (getline(ss, token, delimiter)) {
    slices.push_back(token);
  }
  return slices;
}

inline std::string removeConsecutiveSemicolon(
    const std::string &input) {
  std::string output;
  for (size_t i = 0; i < input.size(); i++) {
    // If the current character is not a semicolon, or if it
    // is a semicolon but is not immediately followed by
    // another semicolon, then copy it to the output string.
    if (input[i] != ';' || i + 1 == input.size() ||
        (i + 1 < input.size() && input[i + 1] != ';')) {
      output += input[i];
    }
  }
  return output;
}

bool tryMutationWithMutators(ysmut::MutatorManager *manager,
    const std::vector<std::string> &mutators) {
  std::string text;
  llvm::raw_string_ostream oss(text);
  manager->setOutStream(oss);

  for (auto &m : mutators) {
    if (!manager->hasMutator(m)) {
      llvm::errs() << "nonexistent mutator " << m << "\n";
      continue;
    }
    manager->setMutator(m);
    if (manager->mutate()) {
      llvm::errs() << "successfully with " << m << "\n";
      if (opt::output == "-")
        llvm::outs()
            << removeConsecutiveSemicolon(oss.str());
      else {
        std::ofstream outfile;
        outfile.open(std::string(opt::output));
        outfile << removeConsecutiveSemicolon(oss.str());
        outfile.close();
      }
      return true;
    } else if (oss.str().size())
      llvm::errs() << "error with " << m << ":\n"
                   << removeConsecutiveSemicolon(oss.str())
                   << "\n";
  }
  return false;
}

int main(int argc, char **argv) {
  using namespace ysmut;
  llvm::cl::HideUnrelatedOptions(opt::mutOpts);
  llvm::cl::ParseCommandLineOptions(argc, argv, "YSMut");
  auto *manager = MutatorManager::getInstance();
  manager->srand(opt::seed);
  llvm::errs() << "use seed " << unsigned(opt::seed)
               << "\n";

  if (opt::listMutators) {
    manager->printMutators();
    return 0;
  }

  MutationRange range;

  if (opt::mutationRange.getNumOccurrences()) {
    try {
      json jrange = json::parse(opt::mutationRange);
      assert(jrange.is_array() && jrange.size() == 2);
      range = MutationRange::from_json(jrange);
    } catch (nlohmann::json::parse_error &e) {}
  }

  range.filename = opt::input;

  manager->setOutputFile(opt::output);
  manager->setSourceFile(opt::input);
  manager->setMutationRange(range);

  if (opt::tryMutators.getNumOccurrences()) {
    return !tryMutationWithMutators(
        manager, splitString(opt::tryMutators, ','));
  } else if (opt::randomlyTryAllMutators) {
    auto mutators = manager->getAllMutators();
    std::shuffle(mutators.begin(), mutators.end(),
        manager->getRandomGenerator());
    return !tryMutationWithMutators(manager, mutators);
  } else {
    return !tryMutationWithMutators(
        manager, {std::string(opt::mutator)});
  }
  return 1;
}
