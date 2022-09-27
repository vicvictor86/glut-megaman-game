CPP    = g++
WINDRES= windres
RM     = rm -f
OBJS   = megaman.o \
         AppResource.res

LIBS   = -mwindows -static -lglut32 -lglu32 -lopengl32 -lwinmm -lgdi32
CFLAGS = -DGLUT_STATIC

.PHONY: Jogo3D.exe clean clean-after

all: Jogo3D.exe

clean:
	$(RM) $(OBJS) Jogo3D.exe

clean-after:
	$(RM) $(OBJS)

Jogo3D.exe: $(OBJS)
	$(CPP) -Wall -s -o $@ $(OBJS) $(LIBS)

megaman.o: megaman.cpp classes/fire.h classes/player.h classes/Collision.h classes/Enemy.h
	$(CPP) -Wall -s -c $< -o $@ $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

