
CC	= gcc
CFLAG = -fPIC -fprofile-arcs -ftest-coverage -lgcov
RM	= rm -rf

Simulation_Code.o: Simulation_Code.c
	$(CC) -c -Wall -Werror Simulation_Code.c

Test_ACC.o: Test_ACC.c
	$(CC) $(CFLAG) -c -Wall -Werror Test_ACC.c

build_simu: Simulation_Code.o ## Make build
	$(CC) -c -Wall -Werror Simulation_Code.c
	$(CC) -o Simulation Simulation_Code.o -lm
	./Simulation

build_test: Test_ACC.o ## Make build 
	$(CC) $(CFLAG) -c -Wall -Werror Test_ACC.c
	$(CC) $(CFLAG) -o Test Test_ACC.o -lcunit
	./Test

coverage: ## Make coverage
	gcov Test_ACC.c

deps: ## Install dependencies of gcovr
	sudo apt-get install lcov clang-format
	pip install gcovr

gcovr-report: coverage ## Generate gcovr report ## If exists the directory, remove them first.
	mkdir gcovr-report
	gcovr --root . --html --html-details --output gcovr-report/coverage.html
	
clean: ## Clean all generated files
	$(RM) main *.out *.o *.so *.gcno *.gcda *.gcov lcov-report gcovr-report

install cunit:
	sudo apt update
	sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
