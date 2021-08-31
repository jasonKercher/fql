#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdbool.h>

/* If the condition returns FQL_FAIL, then we
 * want to pass that up. A program relying on
 * user input has to check everything...
 */
#define try_(condition_)                  \
	({                                \
		long ret_ = (condition_); \
		if (ret_ == FQL_FAIL) {   \
			return FQL_FAIL;  \
		}                         \
		ret_;                     \
	})

#define try_deref_(condition_)                              \
	({                                                  \
		long ret_ = (condition_);                   \
		if (ret_ == FQL_FAIL || ret_ == FQL_NULL) { \
			return ret_;                        \
		}                                           \
		ret_;                                       \
	})

#define fail_if_(condition_)              \
	({                                \
		long ret_ = (condition_); \
		if (ret_) {               \
			return FQL_FAIL;  \
		}                         \
		ret_;                     \
	})

#define overflow_safe_add_i(n0_, n1_)                                    \
	({                                                               \
		if (n0_ > 0 && n1_ > 0                                   \
		    && (unsigned long)n0_ + (unsigned long)n1_           \
		               > (unsigned long)LONG_MAX) {              \
			fputs("arithmetic overflow detected\n", stderr); \
			return FQL_FAIL;                                 \
		} else if (n0_ < 0 && n1_ < 0                            \
		           && (unsigned long)-n0_ + (unsigned long)-n1_  \
		                      > (unsigned long)LONG_MAX + 1) {   \
			fputs("arithmetic overflow detected\n", stderr); \
			return FQL_FAIL;                                 \
		}                                                        \
		n0_ + n1_;                                               \
	})

#define overflow_safe_minus_i(n0_, n1_)                                  \
	({                                                               \
		if (n0_ > 0 && n1_ < 0                                   \
		    && (unsigned long)n0_ + (unsigned long)-n1_          \
		               > (unsigned long)LONG_MAX) {              \
			fputs("arithmetic overflow detected\n", stderr); \
			return FQL_FAIL;                                 \
		} else if (n0_ < 0 && n1_ > 0                            \
		           && (unsigned long)-n0_ + (unsigned long)n1_   \
		                      > (unsigned long)LONG_MAX + 1) {   \
			fputs("arithmetic overflow detected\n", stderr); \
			return FQL_FAIL;                                 \
		}                                                        \
		n0_ - n1_;                                               \
	})

#define overflow_safe_multiply_i(n0_, n1_)                               \
	({                                                               \
		unsigned long u0_ = 0;                                   \
		unsigned long u1_ = 0;                                   \
		bool neg_ = false;                                       \
		if (n0_ < 0) {                                           \
			u0_ = n0_ * -1;                                  \
			neg_ = true;                                     \
		} else {                                                 \
			u0_ = n0_;                                       \
		}                                                        \
		if (n1_ < 0) {                                           \
			u1_ = n1_ * -1;                                  \
			neg_ = !neg_;                                    \
		} else {                                                 \
			u1_ = n1_;                                       \
		}                                                        \
		unsigned long result_ = u0_ * u1_;                       \
		if (u0_ != 0                                             \
		    && (result_ / u0_ != u1_                             \
		        || result_ > (unsigned long)LONG_MAX + neg_)) {  \
			fputs("arithmetic overflow detected\n", stderr); \
			return FQL_FAIL;                                 \
		}                                                        \
		(neg_) ? -1 * result_ : result_;                         \
	})

#endif /* MISC_H */
