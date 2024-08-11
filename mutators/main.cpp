#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include "MutationServer.h"
#include "MutatorManager.h"
#include "fmt/fmtlog.h"
#include "json.hpp"
#include "utils.h"

using json = nlohmann::ordered_json;

namespace opt {
using namespace llvm;

cl::OptionCategory mutOpts("options for MetaMut");

cl::opt<std::string> mutator("mutator",
    cl::desc("Specify the mutator to apply, eg. "
             "-mutator=s.add-initialization,s.add-suffix-unaryop will try "
             "these two mutators until success"),
    cl::cat(mutOpts));

cl::opt<std::string> output(
    "o", cl::desc("Specify the output file"), cl::cat(mutOpts));
cl::opt<std::string> input(
    "i", cl::desc("Specify the input file"), cl::cat(mutOpts));

cl::opt<unsigned> seed(
    "seed", cl::init(0), cl::desc("Specify the random seed"), cl::cat(mutOpts));

cl::opt<bool> listMutators(
    "list-mutators", cl::desc("List all mutators"), cl::cat(mutOpts));

cl::opt<bool> syntaxOnly("syntax-only", cl::desc("Perform syntax checks only"),
    cl::init(false), cl::cat(mutOpts));

cl::opt<bool> verbose(
    "v", cl::desc("Verbose on"), cl::init(false), cl::cat(mutOpts));

cl::opt<bool> printMutationState("print-mutations",
    cl::desc("Print each mutation's results"), cl::init(false),
    cl::cat(mutOpts));

cl::opt<bool> randomlyTryAllMutators("randomly-try-all-mutators",
    cl::init(false), cl::desc("Try all specific mutators in specified order"),
    cl::cat(mutOpts));

} // namespace opt

std::vector<std::string> splitString(const std::string &str, char delimiter) {
  std::stringstream ss(str);
  std::string token;
  std::vector<std::string> slices;
  while (getline(ss, token, delimiter)) { slices.push_back(token); }
  return slices;
}

bool tryMutationWithMutators(
    llvm::StringRef srctext, const std::vector<std::string> &mutators) {
  json state = json::array();
  MutationServer server(mutators);

  server.setup_isrc(srctext);
  state.push_back(
      json::object({{"entry", "record"}, {"seed", unsigned(opt::seed)}}));

  for (auto i = 0u; i < mutators.size(); i++) {
    MutationError error;
    llvm::StringRef output;
    unsigned seed = get_rndgen()();
    std::tie(error, output) = server.apply_mutation(i, seed);
    bool changed = output.size() && output != srctext;

    state.push_back(
        json::object({{"entry", "mutation"}, {"mutator", mutators[i]},
            {"result", formatMutationError(error)}, {"changed", changed}}));
    if (error == MutationError::Success && changed) {
      if (opt::output == "-") {
        llvm::outs() << output;
      } else {
        std::error_code ec;
        llvm::raw_fd_ostream ofs(std::string(opt::output), ec);
        ofs << output;
        ofs.close();
      }
      break;
    }
  }
  if (opt::printMutationState) llvm::errs() << state.dump(2) << "\n";
  return false;
}

int main(int argc, char **argv) {
  llvm::cl::HideUnrelatedOptions(opt::mutOpts);
  llvm::cl::ParseCommandLineOptions(argc, argv, "muss");
  fmtlog::setLogFile(stderr);
  if (!opt::verbose) fmtlog::setLogLevel(fmtlog::OFF);
  logi("Using seed {}", unsigned(opt::seed));
  get_rndgen().seed(opt::seed);

  if (opt::listMutators) {
    MutatorManager::getInstance()->printMutators();
    return 0;
  }

  if (opt::syntaxOnly) return 0;

  // read file contents
  std::ifstream file(std::string(opt::input));
  std::stringstream buffer;

  if (file) {
    buffer << file.rdbuf();
  } else {
    loge("Failed to open the file.");
    return 1;
  }

  std::string buffer_contents = buffer.str();
  llvm::StringRef srctext = buffer_contents;
  if (opt::randomlyTryAllMutators) {
    auto mutators = MutatorManager::getAllMutators();
    std::shuffle(mutators.begin(), mutators.end(), get_rndgen());
    return !tryMutationWithMutators(srctext, mutators);
  } else {
    return !tryMutationWithMutators(srctext, splitString(opt::mutator, ','));
  }
  return 1;
}
