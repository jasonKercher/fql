#include "treewalker.h"

#include "tree/ParseTreeListener.h"
#include "tree/ParseTree.h"
#include "tree/ErrorNode.h"
#include "support/CPPUtils.h"

using namespace antlr4::tree;
using namespace antlrcpp;

/* This is copied straight from the walk defined
 * in IterativeParseTreeWalker.cpp.  This is the
 * default method.  All I did was add the check
 * oh the _walking flag
 */
void TreeWalker::walk(ParseTreeListener* listener,
		      ParseTree* t) const
{

  std::vector<ParseTree *> nodeStack;
  std::vector<size_t> indexStack;

  ParseTree *currentNode = t;
  size_t currentIndex = 0;

  while (_walking && currentNode != nullptr) {
    // pre-order visit
    if (antlrcpp::is<ErrorNode *>(currentNode)) {
      listener->visitErrorNode(dynamic_cast<ErrorNode *>(currentNode));
    } else if (antlrcpp::is<TerminalNode *>(currentNode)) {
      listener->visitTerminal((TerminalNode *)currentNode);
    } else {
      enterRule(listener, currentNode);
    }

    // Move down to first child, if it exists.
    if (!currentNode->children.empty()) {
      nodeStack.push_back(currentNode);
      indexStack.push_back(currentIndex);
      currentIndex = 0;
      currentNode = currentNode->children[0];
      continue;
    }

    // No child nodes, so walk tree.
    do {
      // post-order visit
      if (!antlrcpp::is<TerminalNode *>(currentNode)) {
        exitRule(listener, currentNode);
      }

      // No parent, so no siblings.
      if (nodeStack.empty()) {
        currentNode = nullptr;
        currentIndex = 0;
        break;
      }

      // Move to next sibling if possible.
      if (nodeStack.back()->children.size() > ++currentIndex) {
        currentNode = nodeStack.back()->children[currentIndex];
        break;
      }

      // No next sibling, so move up.
      currentNode = nodeStack.back();
      nodeStack.pop_back();
      currentIndex = indexStack.back();
      indexStack.pop_back();

    } while (_walking && currentNode != nullptr);
  }
}

void TreeWalker::set_walking(bool walking)
{
  _walking = walking;
}
