EXEC_SEQ = pоs
SRC_SEQ = $(EXEC_SEQ).c
EXEC_PAR_PT = paralP
SRC_PAR_PT = $(EXEC_PAR_PT).c
EXEC_PAR_OMP = paralO
SRC_PAR_OMP = $(EXEC_PAR_OMP).c
CFLAGS_OMP = -lm
CFLAGS_PT = -lm
CFLAGS_SEQ = -lm

all: $(EXEC_SEQ) $(EXEC_PAR_PT) $(EXEC_PAR_OMP)
	./$(EXEC_SEQ)
	./$(EXEC_PAR_PT)
	./$(EXEC_PAR_OMP)

$(EXEC_SEQ): $(SRC_SEQ)
	gcc $< -o $@ $(CFLAGS_SEQ)

$(EXEC_PAR_PT): $(SRC_PAR_PT)
	gcc $< -o $@ $(CFLAGS_PT)

$(EXEC_PAR_OMP): $(SRC_PAR_OMP)
	gcc $< -o $@ $(CFLAGS_OMP)

time_pos:
	time ./$(SRC_SEQ)

time_paralO:
	time ./$(SRC_PAR_OMP)

time_paralP:
	time ./$(SRC_PAR_PT)


clean:

	rm -f $(EXEC_SEQ)
	rm -f $(EXEC_PAR_PT)
	rm -f $(EXEC_PAR_OMP)
