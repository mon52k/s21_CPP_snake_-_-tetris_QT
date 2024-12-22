CC = g++
FLAGS = -Wall -Wextra -Werror 
BUILD_DIR = build
DOXYFILE = dvi/Doxyfile

all: clean install test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

install: clean $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a
	$(CC) $(FLAGS) -o $(BUILD_DIR)/Console gui/cli/main_console.cc gui/cli/snake/snake_console.cc gui/cli/tetris/tetris_frontend.c $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a -lncurses
	cd $(BUILD_DIR) && qmake ../gui/desktop
	cd $(BUILD_DIR) && make

uninstall:
	-rm -rf $(BUILD_DIR)

$(BUILD_DIR)/tetris.o: | $(BUILD_DIR)
	$(CC) $(FLAGS) -c brick_game/tetris/tetris_backend.c -o $(BUILD_DIR)/tetris.o

$(BUILD_DIR)/tetris_lib.a: $(BUILD_DIR)/tetris.o
	ar rcs $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.o
	ranlib $(BUILD_DIR)/tetris_lib.a

$(BUILD_DIR)/snake.o: | $(BUILD_DIR)
	$(CC) $(FLAGS) -c brick_game/snake/model/snake.cc -o $(BUILD_DIR)/snake.o

$(BUILD_DIR)/Controller.o: | $(BUILD_DIR)
	$(CC) $(FLAGS) -c brick_game/snake/controller/controller.cc -o $(BUILD_DIR)/Controller.o

$(BUILD_DIR)/snake_lib.a: $(BUILD_DIR)/snake.o $(BUILD_DIR)/Controller.o
	ar rcs $(BUILD_DIR)/snake_lib.a $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.o
	ranlib $(BUILD_DIR)/snake_lib.a

test: clean $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a
	$(CC) -g --coverage $(FLAGS) tests/testTetris.c -o $(BUILD_DIR)/testTetris  $(BUILD_DIR)/tetris_lib.a -lcheck  # -lpthread -lrt -lm -lsubunit
	$(CC) -g --coverage $(FLAGS) tests/testSnake.cc -o $(BUILD_DIR)/testSnake  $(BUILD_DIR)/snake_lib.a -lstdc++ -pthread -lgtest -lgcov -lm
	$(BUILD_DIR)/testTetris
	$(BUILD_DIR)/testSnake


gcov_report: clean $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a
	rm -f *.g*
	$(CC) $(FLAGS) brick_game/tetris/tetris_backend.c tests/testTetris.c -o build/testTetris $(BUILD_DIR)/tetris_lib.a -lcheck --coverage -lncurses
	./build/testTetris
	$(CC) $(FLAGS) brick_game/snake/model/snake.cc brick_game/snake/controller/controller.cc tests/testSnake.cc -o build/testSnake $(BUILD_DIR)/snake_lib.a -lstdc++ -pthread -lgtest -lgcov -lm --coverage -lncurses
	./build/testSnake
	lcov --ignore-errors inconsistent --ignore-errors unused -t "Tetris Test Coverage" -o rep_tetris.info -c -d . --exclude "*c++/13/*" --exclude "*gtest/*" --exclude "*report/*"
	lcov --ignore-errors inconsistent --ignore-errors unused -t "Snake Test Coverage" -o rep_snake.info -c -d . --exclude "*c++/13/*" --exclude "*gtest/*" --exclude "*report/*"
	lcov -a rep_tetris.info -a rep_snake.info -o rep.info
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcda *.gcno

clean:
	rm -rf $(BUILD_DIR) $(OBJECTS) *.info gcov_report score.txt tetris.tar.gz report doxygen_report

dvi: clean
	doxygen doc/Doxyfile
	open doxygen_report/html/index.html

dist: clean install test
	mkdir -p $(BUILD_DIR)/dist
	cp $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a $(BUILD_DIR)/Console $(BUILD_DIR)/testTetris $(BUILD_DIR)/testSnake $(BUILD_DIR)/dist/
	tar -czf $(BUILD_DIR)/console_apps.install.tar.gz -C $(BUILD_DIR)/dist .
	rm -rf $(BUILD_DIR)/dist

format: clean
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -i */*/*.c */*/*/*.c */*/*/*.h */*/*.h */*/*.cc */*/*/*.cc  */*/*/*.h */*/*.cc  */*/*.h
	rm .clang-format

style:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -n */*/*.c */*/*/*.c */*/*/*.h  */*/*.h */*/*.cc */*/*/*.cc  */*/*/*.h */*/*.cc  */*/*.h
	rm .clang-format

leaks: test
ifeq ($(shell uname -s), Linux)
	valgrind  --tool=memcheck --leak-check=yes  --show-leak-kinds=all ./$(BUILD_DIR)/testSnake   2>&1 | awk '/ERROR SUMMARY:/ { print }'
	valgrind  --tool=memcheck --leak-check=yes  --show-leak-kinds=all ./$(BUILD_DIR)/testTetris   2>&1 | awk '/ERROR SUMMARY:/ { print }'

else
	leaks --atExit -- 	./build/testSnake
	leaks --atExit -- 	./build/testTetris
endif