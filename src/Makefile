ifeq ($(shell uname), Linux)
	LIBS = -lcheck -lsubunit -lrt -lm -lpthread
else ifeq ($(shell uname), Darwin)
	LIBS = -lcheck
endif
CURDIR=$(shell pwd)
DISTDIR=~/Desktop/SmartCalc
all: s21_calc.a tests gcov_report install dist dvi clean
s21_calc.a:
	@gcc -c back/list.c -o list.o
	@gcc -c back/calc.c -o calc.o
	@gcc -c back/stackChar.c -o stackChar.o
	@gcc -c back/stackDouble.c -o stackDouble.o
	@ar -crs s21_calc.a calc.o list.o stackChar.o stackDouble.o
install:
	@-mkdir $(DISTDIR)
	@-cd $(DISTDIR) && qmake6 $(CURDIR)/front/Calc.pro
	@-cd $(DISTDIR) && make
	@-cd $(DISTDIR) && make clean
	@-cd $(DISTDIR) && rm -f Makefile
	@-cd $(DISTDIR) && rm -f *.stash
uninstall:
	@rm -rf $(DISTDIR)
clean:
	@rm -f *.o
	@rm -f *.out
	@rm -f *.a
	@rm -f tests
	@rm -f *.gcda
	@rm -f *.gcno
	@rm -f coverage_tests
	@rm -f *.info
	@rm -f tests.c
dvi:
	@echo "\n\nДокументация по SmartCalc\n\n"
	@cp calc_doc $(DISTDIR)/calc_doc.dvi
	@-cd $(DISTDIR) && cat calc_doc.dvi
dist:
	@mkdir s21_calc
	@cp -R back s21_calc/back
	@cp -R front s21_calc/front
	@cp Makefile s21_calc/Makefile
	@cp calc_doc s21_calc/calc_doc
	@tar -cf s21_calc.tar s21_calc
	@gzip s21_calc.tar
	@rm -rf s21_calc
tests: s21_calc.a make_tests
	@gcc tests.c s21_calc.a $(LIBS) -o tests
	./tests
make_tests:
	@checkmk clean_mode=1 tests.check > tests.c
	@clang-format -i tests.c
gcov_report: make_tests s21_calc.a
	@gcc --coverage tests.c -o coverage_tests back/*.c $(LIBS)
	./coverage_tests
	@lcov -t "coverage_tests" -o coverage_tests.info -c -d .
	@genhtml -o report coverage_tests.info