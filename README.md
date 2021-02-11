### PHP Extensions

With the modern 7.x and later PHP codebase writing a fully self-contained
extension is much easier than it was on the older 5.x codebase, however
most tutorials and even most books still rely on the older `phpize` based
approach which generates literally megabytes of automated scripting.

Using `php-config` directly allows for much smaller, more readable Makefile
creation, without relying on large automake and related scripting to compile
what is often only a handful of source files.

### What is wdf85?

The name is based on a typography failure I've encountered with my actual
name "Wolf" where the "ol" sequence failed to kern correctly because a much
older FreeType library was installed with all the expired patent concerns
still enabled.

The actual tool itself is a minor variant on
[Ascii85](https://en.wikipedia.org/wiki/Ascii85) encoding, adjusting the
output character map to make it
[cookie safe](https://stackoverflow.com/a/1969339) by adding characters
unused in the default Ascii85 encoding to replace earlier ones.

A single `strtr()` call on the "encoded" state can convert the output to
normal Ascii85 encoding if needed. Otherwise this is merely a 'fully
functional toy' to aide in showing how this approach using a classical
Makefile would work while still including full validation and code
coverage tests.
