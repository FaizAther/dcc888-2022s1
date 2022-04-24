// To Dot, Ufmg module
// Author: ather
//

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include <vector>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
  struct Ufmg : public FunctionPass {
    static char ID;
    Ufmg() : FunctionPass(ID) {}

    int runOnBasicBlocks(BasicBlock *bb, std::vector<BasicBlock *> &visited, raw_fd_ostream *stream) {
      if (bb == NULL)
        return 0;
      for (BasicBlock *bi : visited) {
        if (bb == bi)
          return 0;
      }
      runOnBasicBlock(bb, stream);
      visited.push_back(bb);
      for (BasicBlock *bs : successors(bb)) {
        *stream << "\tNode" << (void *)bb << " -> ";
        *stream << "\tNode" << (void *)bs << ";";
        *stream << "\n";
      }
      for (BasicBlock *bs : successors(bb)) {
        runOnBasicBlocks(bs, visited, stream);
      }
      return 0;
    }

    bool runOnBasicBlock(BasicBlock *bb, raw_fd_ostream *stream) {
      *stream << "\tNode" << (void *)bb << "[shape=record, label=\"{BB";
      *stream << bb->getNameOrAsOperand().substr(1);
      *stream << ":";
      for (BasicBlock::InstListType::iterator il = bb->getInstList().begin() \
        , e = bb->getInstList().end(); e != il; il++) {
        Instruction *itn = &*il;
        if (itn->getOpcode() == Instruction::Alloca || itn->getOpcode() == Instruction::Store) {
          continue;
        }
        // *stream << "\\l  " << *itn;
        *stream << "\\l  ";
        if (itn->getOpcode() != Instruction::Ret && itn->getOpcode() != Instruction::Br) {
          if (!(itn->getOpcode() == Instruction::Call && itn->getType()->isVoidTy())) {
            itn->printAsOperand(*stream, false);
            *stream << itn->getName() << " = ";
          }/* else {
            errs() << *itn->getType() << "XXX\n";
          }*/
        }
        *stream << itn->getOpcodeName() << " ";
        // if (itn->getOpcode() == Instruction::Call) {
        //   errs() << "Is a function call\n" << *itn << "\n";
        // }
        for (unsigned int i = 0; i < itn->getNumOperands(); i++) {
          Value *iop = itn->getOperand(i);
          if (itn->getOpcode() == Instruction::Call) {
            if (i == 0) {
              iop = itn->getOperand(itn->getNumOperands() - 1);
            } else {
              iop = itn->getOperand(i - 1);
            }
            if (iop->hasName()) {
              *stream << "@" << iop->getName();
              *stream << "(";
            } else {
              if (auto *cexpr = dyn_cast<ConstantExpr>(iop)) {
                if (cexpr->isCast()) {
                   *stream << cexpr->getOperand(0)->getName();
                   *stream << "(";
                }
              } else { // Unnamed
                *stream << iop->getNameOrAsOperand();
                if (i != itn->getNumOperands() - 1) {
                  *stream << ", ";
                }
              }
            }
            if (i == itn->getNumOperands() - 1) {
              *stream << ")";
            }
          } else {
            if (auto *cexpr = dyn_cast<ConstantExpr>(iop)) {
              if (cexpr->getOpcode() == Instruction::GetElementPtr) {
                *stream << cexpr->getOpcodeName() << " ";
                for (unsigned int gis = 0; gis < cexpr->getNumOperands(); gis++) {
                  *stream << cexpr->getOperand(gis)->getNameOrAsOperand();
                  if (gis != cexpr->getNumOperands() - 1){
                    *stream << ", ";
                  }
                }
              } else {
                iop->printAsOperand(*stream, false);
              }
            } else {
              iop->printAsOperand(*stream, false);
            }
            // *stream << " " << *itn->getOperand(i);
            if (i != itn->getNumOperands() - 1) {
              *stream << ", ";
            }
          }
          // *stream << " " << *itn->getOperand(i);
        }
      }
      *stream << "\\l}\"];\n";
      return false;
    }

    bool runOnFunction(Function &F) override {
      std::vector<BasicBlock *> visited;
      std::error_code EC;
      std::string name = F.getName().str();
      name += ".udot";
      raw_fd_ostream *stream = new raw_fd_ostream(name, EC);
      errs() << "Writing cfg to " << name << "\n";
      visited.clear();
      *stream << "digraph \"CFG for \'" << F.getName() << "\' function\" {\n";
      *stream << "\tlabel=\"CFG for \'" << F.getName() << "\' function\";\n\n";
      runOnBasicBlocks(&F.getEntryBlock(), visited, stream);
      *stream << "}\n";
      stream->flush();
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
