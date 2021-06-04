#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdbool.h>

/* If the condition returns FQL_FAIL, then we
 * want to pass that up. A program relying on
 * user input has to check everything...
 */
#define try_(condition_)                                                       \
	({                                                                     \
		int ret_ = (condition_);                                       \
		if (ret_ == FQL_FAIL) {                                        \
			return FQL_FAIL;                                       \
		}                                                              \
		ret_;                                                          \
	})

#define fail_if_(condition_)                                                   \
	({                                                                     \
		int ret_ = (condition_);                                       \
		if (ret_) {                                                    \
			return FQL_FAIL;                                       \
		}                                                              \
		ret_;                                                          \
	})

#define overflow_safe_add_i(n0_, n1_)                                          \
	({                                                                     \
		if (n0_ > 0 && n1_ > 0                                         \
		    && (unsigned long)n0_ + (unsigned long)n1_                 \
		               > (unsigned long)LONG_MAX) {                    \
			fputs("arithmetic overflow detected\n", stderr);       \
			return FQL_FAIL;                                       \
		} else if (n0_ < 0 && n1_ < 0                                  \
		           && (unsigned long)-n0_ + (unsigned long)-n1_        \
		                      > (unsigned long)LONG_MAX + 1) {         \
			fputs("arithmetic overflow detected\n", stderr);       \
			return FQL_FAIL;                                       \
		}                                                              \
		n0_ + n1_;                                                     \
	})

#define overflow_safe_minus_i(n0_, n1_)                                        \
	({                                                                     \
		if (n0_ > 0 && n1_ < 0                                         \
		    && (unsigned long)n0_ + (unsigned long)-n1_                \
		               > (unsigned long)LONG_MAX) {                    \
			fputs("arithmetic overflow detected\n", stderr);       \
			return FQL_FAIL;                                       \
		} else if (n0_ < 0 && n1_ > 0                                  \
		           && (unsigned long)-n0_ + (unsigned long)n1_         \
		                      > (unsigned long)LONG_MAX + 1) {         \
			fputs("arithmetic overflow detected\n", stderr);       \
			return FQL_FAIL;                                       \
		}                                                              \
		n0_ - n1_;                                                     \
	})

#define overflow_safe_multiply_i(n0_, n1_)                                     \
	({                                                                     \
		unsigned long u0 = 0;                                          \
		unsigned long u1 = 0;                                          \
		_Bool neg = false;                                             \
		if (n0 < 0) {                                                  \
			u0 = n0 * -1;                                          \
			neg = true;                                            \
		} else {                                                       \
			u0 = n0;                                               \
		}                                                              \
		if (n1 < 0) {                                                  \
			u1 = n1 * -1;                                          \
			neg = !neg;                                            \
		} else {                                                       \
			u1 = n1;                                               \
		}                                                              \
		unsigned long result = u0 * u1;                                \
		if (u0 != 0                                                    \
		    && (result / u0 != u1                                      \
		        || result > (unsigned long)LONG_MAX + neg)) {          \
			fputs("arithmetic overflow detected\n", stderr);       \
			return FQL_FAIL;                                       \
		}                                                              \
		(neg) ? -1 * result : result;                                  \
	})

#endif  /* MISC_H */
