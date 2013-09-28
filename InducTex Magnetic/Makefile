# This should be set to the Sun Studio compiler... usually cc on Solaris systems
 CC = cc
 LD = $(CC)

# don't modify this line: -xopenmp turns on OpenMP
 7 CCFLAGS = -c -xopenmp -xO3
 8 LDFLAGS = -xopenmp -xO3

 # add other CFLAGS here
 # CCFLAGS += -DEXAMPLE

# list your binaries here
 BINARIES = main JToB FileReader DataTypes CoordinatesCalulator
 OPATH = .generated
 LINK_MESSAGE = "\033[32;1m\n***Making binary \033[33m%s\033[32m...\n\033[m"
 COMPILE_MESSAGE = "\033[32;1m\n*Compiling \033[33m%s\033[32m...\n\033[m"

# list OBJFILES here for each binary
 OBJFILES_MAIN = $(OPATH)/main.o \
                 $(OPATH)/JToB.o \
                 $(OPATH)/FileReader .o \
                 $(OPATH)/DataTypes .o \
                 $(OPATH)/CoordinatesCalulator.o
# binary targets are like this
main: $(OPATH) $(OBJFILES_MAIN)
     @printf $(LINK_MESSAGE) "main"
     $(LD) $(LDFLAGS) $(OBJFILES_MAIN) -o main

$(OPATH)/main.o: main.c
     @printf $(CpthreadsILE_MESSAGE) "main.c"
     $(CC) $(CCFLAGS) main.c -o $(OPATH)/main.o

$(OPATH)/JToB.o: JToB.c
     @printf $(CpthreadsILE_MESSAGE) "JToB.c"
     $(CC) $(CCFLAGS) JToB.c -o $(OPATH)/JToB.o

$(OPATH)/JToB.o: FileReader.c
     @printf $(CpthreadsILE_MESSAGE) "FileReader.c"
     $(CC) $(CCFLAGS) FileReader.c -o $(OPATH)/FileReader.o

$(OPATH)/DataTypes.o: DataTypes.c
     @printf $(CpthreadsILE_MESSAGE) "DataTypes.c"
     $(CC) $(CCFLAGS) DataTypes.c -o $(OPATH)/DataTypes.o


$(OPATH)/CoordinatesCalulator.o: CoordinatesCalulator.c
     @printf $(CpthreadsILE_MESSAGE) "CoordinatesCalulator.c"
     $(CC) $(CCFLAGS) CoordinatesCalulator.c -o $(OPATH)/CoordinatesCalulator.o