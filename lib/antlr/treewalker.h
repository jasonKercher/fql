#ifndef TREE_WALKER_H
#define TREE_WALKER_H

#include "tree/ParseTreeWalker.h"

class TreeWalker : public antlr4::tree::ParseTreeWalker
{
      private:
	bool _walking = false;

      public:
	void walk(antlr4::tree::ParseTreeListener*,
	          antlr4::tree::ParseTree*) const override;
	void set_walking(bool);
};

#endif /* TREE_WALKER_H */
