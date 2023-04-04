NAME = lab3_1
SRC = $(NAME).c
EXEC = $(NAME).exe

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -mwindows -lcomdlg32 -lgdi32

build_and_run: $(EXEC)
	wine $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS)

clean:
	rm -f $(EXEC)
