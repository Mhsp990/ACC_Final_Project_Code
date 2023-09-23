
CC	= gcc
CFLAG = -fPIC -fprofile-arcs -ftest-coverage -lgcov
RM	= rm -rf

Test_ACC.o: Test_ACC.c
	$(CC) $(CFLAG) -c -Wall -Werror Test_ACC.c

build: Test_ACC.o ## Make build 
	$(CC) $(CFLAG) -c -Wall -Werror Test_ACC.c
	$(CC) $(CFLAG) -o main Test_ACC.o -lcunit

coverage:
	gcov Test_ACC.c


deps: ## Install dependencies of gcovr
	sudo apt-get install lcov clang-format
	pip install gcovr

gcovr-report: coverage ## Generate gcovr report ## If exists the directory, remove them first.
	mkdir gcovr-report
	gcovr --root . --html --html-details --output gcovr-report/coverage.html

clean: ## Clean all generated files
	$(RM) main *.out *.o *.so *.gcno *.gcda *.gcov lcov-report gcovr-report

lint: ## Lint code with clang-format
	clang-format -i --style=LLVM *.c *.h