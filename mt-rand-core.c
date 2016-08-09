/*
  Copyright (C) 2016 by Syohei YOSHIDA

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _BSD_SOURCE
#include <stdio.h>
#include <time.h>
#include <emacs-module.h>

#include "mt19937ar.h"

int plugin_is_GPL_compatible;

static bool is_initialized = false;

static void
mt_rand_srand(emacs_env *env, emacs_value seed)
{
	unsigned long s;

	if (env->is_not_nil(env, seed)) {
		s = (unsigned long)(env->extract_integer(env, seed));
	} else {
		s = (unsigned long)(time(NULL) + genrand_int32());
	}

	init_genrand(s);
}

static emacs_value
Fmt_rand_random(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	if (!is_initialized)
		mt_rand_srand(env, env->intern(env, "nil"));

	emacs_value ret;
	if (!env->is_not_nil(env, args[0])) {
		ret = env->make_float(env, genrand_real1());
	} else {
		emacs_value type = env->type_of(env, args[0]);
		unsigned long max;
		if (env->eq(env, type, env->intern(env, "integer"))) {
			max = (unsigned long)(env->extract_integer(env, args[0]));
		} else if (env->eq(env, type, env->intern(env, "float"))) {
			max = (unsigned long)(env->extract_float(env, args[0]));
		}

		if (max == 0)
			return env->make_integer(env, 0);

		unsigned long v = genrand_int32() % max;
		ret = env->make_integer(env, v);
	}

	return ret;
}

static emacs_value
Fmt_rand_srand(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	unsigned long seed;

	if (env->is_not_nil(env, args[0])) {
		seed = (unsigned long)(env->extract_integer(env, args[0]));
	} else {
		seed = genrand_int32();
	}

	init_genrand(seed);
	is_initialized = true;
	return env->intern(env, "t");
}

static void
bind_function(emacs_env *env, const char *name, emacs_value Sfun)
{
	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qsym = env->intern(env, name);
	emacs_value args[] = { Qsym, Sfun };

	env->funcall(env, Qfset, 2, args);
}

static void
provide(emacs_env *env, const char *feature)
{
	emacs_value Qfeat = env->intern(env, feature);
	emacs_value Qprovide = env->intern (env, "provide");
	emacs_value args[] = { Qfeat };

	env->funcall(env, Qprovide, 1, args);
}

int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env *env = ert->get_environment(ert);

#define DEFUN(lsym, csym, amin, amax, doc, data) \
	bind_function (env, lsym, env->make_function(env, amin, amax, csym, doc, data))

	DEFUN("mt-rand-core-random", Fmt_rand_random, 1, 1, NULL, NULL);
	DEFUN("mt-rand-core-srand", Fmt_rand_srand, 1, 1, NULL, NULL);

#undef DEFUN

	provide(env, "mt-rand-core");
	return 0;
}

/*
  Local Variables:
  c-basic-offset: 8
  indent-tabs-mode: t
  End:
*/
