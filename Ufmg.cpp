// Hello, Ufmg module
// Author: ather
//

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include <vector>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
  struct Ufmg : public FunctionPass {
    static char ID;
    Ufmg() : FunctionPass(ID) {}

    int runOnBasicBlocks(BasicBlock *bb, std::vector<BasicBlock *> &visited) {
      if (bb == NULL)
        return 0;
      for (BasicBlock *bi : visited) {
        if (bb == bi)
          return 0;
      }
      runOnBasicBlock(bb);
      visited.push_back(bb);
      for (BasicBlock *bs : successors(bb)) {
        errs() << "\tNode" << (void *)bb << " -> ";
        errs() << "\tNode" << (void *)bs << ";";
        errs() << "\n";
      }
      for (BasicBlock *bs : successors(bb)) {
        runOnBasicBlocks(bs, visited);
      }
      return 0;
    }

    bool runOnBasicBlock(BasicBlock *bb) {
      errs() << "\tNode" << (void *)bb << "[shape=record, label=\"{";
      bb->printAsOperand(errs(), false);
      errs() << ":";
      for (BasicBlock::InstListType::iterator il = bb->getInstList().begin(), e = bb->getInstList().end(); e != il; il++) {
        Instruction *itn = &*il;
        errs() << "\\l  " << *itn;
      }
      errs() << "\\l}\"];\n";
      return false;
    }

    bool runOnFunction(Function &F) override {
      std::vector<BasicBlock *> visited;
      visited.clear();
      errs() << "digraph \"CFG for \'" << F.getName() << "\' function\" {\n";
      errs() << "\tlabel=\"CFG for \'" << F.getName() << "\' function\";\n\n";
      runOnBasicBlocks(&F.getEntryBlock(), visited);
      errs() << "}\n";
      return false;
    }
  }; // end of struct Ufmg
}    // end of anon namespace

char Ufmg::ID = 0;

static RegisterPass<Ufmg> X("ufmg", "Hello, Ufmg Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

static llvm::RegisterStandardPasses Y(
    llvm::PassManagerBuilder::EP_EarlyAsPossible,
    [](const llvm::PassManagerBuilder &Builder,
        llvm::legacy::PassManagerBase &PM) { PM.add(new Ufmg()); });
