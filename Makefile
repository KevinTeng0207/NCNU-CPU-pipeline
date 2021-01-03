CC = gcc
LD = gcc
CFLAGS = -g -lm
LDFLAGS =
RM = rm -f
PROG = NCNU_CPU_SIM
OBJS = 	main.o	\
	inst_process.o\
	machine.o	\
                     pipeline.o

# top-level rule, to compile everything.
all: $(PROG)

# rule to link the program
$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) -g -lm $(OBJS) -o $(PROG)

# eve: what is this?
.c.o:
	$(CC) $(CFLAGS) -c $<

main.o: main.c main.h machine.h pipeline.h

machine.o: machine.h 

inst_process.o: inst_process.h machine.h pipeline.h sim.h

pipeline.o: pipeline.h machine.h

#Perturb.o: Perturb.c Perturb.h main.h io.h

#TDDG.o: TDDG.c TDDG.h io.h main.h

#SW_Synthesis.o: SW_Synthesis.c SW_Synthesis.h io.h main.h Perturb.h 

#Partition_Algorithm.o: Partition_Algorithm.c Partition_Algorithm.h TDDG.h io.h main.h 

#SA.o: SA.cpp SA.h myHeader.h

#ACG.o: ACG.cpp ACG.h myHeader.h

#CTG.o: CTG.cpp CTG.h myHeader.h

#energyModel.o: energyModel.cpp energyModel.h myHeader.h

#GraphNode.o: GraphNode.cpp GraphNode.h myHeader.h

#Tile.o: Tile.cpp Tile.h myHeader.h

#Link.o: Link.cpp Link.h myHeader.h

#PE.o: PE.cpp PE.h

#PE_Library.o: PE_Library.cpp PE_Library.h myHeader.h

#myScheduler.o: myScheduler.cpp myScheduler.h

#mySolution.o: mySolution.cpp mySolution.h

# rule for cleaning re-compilable files.
clean:
	$(RM) $(PROG) $(OBJS)
