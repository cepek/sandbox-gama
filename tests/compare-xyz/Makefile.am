# https://www.gnu.org/software/automake/manual/html_node/Scripts.html

# CLEANFILE =
# bin_scripts = compare-xyz
# CLEANFILES = $(bin_scripts)

EXTRA_DIST = CMakeLists.txt \
             compare-xyz.in

TESTS = compare-xyz.sh

COMPARE_XYZ_SRC     = $(srcdir)/src
COMPARE_XYZ_INPUT   = $(srcdir)/input
COMPARE_XYZ_RESULTS = $(srcdir)/results/$(PACKAGE_VERSION)
COMPARE_XYZ_OTHERS  = Makefile $(COMPARE_XYZ_RESULTS)

do_subst = sed \
-e 's,[@]abs_top_builddir[@],$(abs_top_builddir),g' \
-e 's,[@]abs_top_srcdir[@],$(abs_top_srcdir),g' \
-e 's,[@]PACKAGE_VERSION[@],$(PACKAGE_VERSION),g' \
-e 's,[@]INPUT_FILES[@],$(INPUT_FILES),g' \
-e 's,[@]GAMA_INPUT[@],$(GAMA_INPUT),g' \
-e 's,[@]GAMA_RESULTS[@],$(GAMA_RESULTS),g' \
-e 's,[@]GAMA_XML[@],$(top_srcdir)/xml,g' \
-e 's,[@]BUG_FILES[@],$(BUG_FILES),g' \
-e 's,[@]CTU_FILES[@],$(CTU_FILES),g' \
-e 's,[@]KRUMM_FILES[@],$(KRUMM_FILES),g' \
-e 's,[@]FROMTODH_FILES[@],$(FROMTODH_FILES),g' \
-e 's,[@]YAML_FILES[@],$(YAML_FILES),g' \
-e 's,[@]GNU_GAMA_LOCAL_TEST_XMLLINT[@],$(GNU_GAMA_LOCAL_TEST_XMLLINT),g'

clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	-rm -rf $(COMPARE_XYZ_RESULTS) $(TESTS)

$(COMPARE_XYZ_RESULTS):
	$(MKDIR_P) $(COMPARE_XYZ_RESULTS)

compare-xyz.sh: $(srcdir)/compare-xyz.in
	$(do_subst) < $(srcdir)/compare-xyz.in \
	            > compare-xyz.sh
	chmod +x compare-xyz.sh
