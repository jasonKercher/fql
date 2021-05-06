#include "treewalker.h"

#include "tree/ParseTreeListener.h"
#include "tree/ParseTree.h"
#include "tree/ErrorNode.h"
#include "support/CPPUtils.h"

using namespace antlr4::tree;
using namespace antlrcpp;

void TreeWalker::walk(ParseTreeListener* listener,
		      ParseTree* t) const
{
  if (is<ErrorNode *>(t)) {
    listener->visitErrorNode(dynamic_cast<ErrorNode *>(t));
    return;
  } else if (is<TerminalNode *>(t)) {
    listener->visitTerminal(dynamic_cast<TerminalNode *>(t));
    return;
  }

  enterRule(listener, t);
  for (auto &child : t->children) {
    if (!_walking) {
      break;
    }
    walk(listener, child);
  }
  exitRule(listener, t);
}

void TreeWalker::set_walking(bool walking)
{
  _walking = walking;
}
