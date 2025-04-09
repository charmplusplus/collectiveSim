CHARMC ?= charmc
FLAGS = -g -Wall -O2
TYPE ?= RING
MESSAGE_DIR=./message
ZEROCOPY_DIR=./zerocopy


message:
	$(CHARMC) $(MESSAGE_DIR)/message.ci
	@mv message_sim.decl.h $(MESSAGE_DIR)/message_sim.decl.h
	@mv message_sim.def.h $(MESSAGE_DIR)/message_sim.def.h
	$(CHARMC) -o message_sim $(MESSAGE_DIR)/message.cc
	@echo "Built ping-pong using message passing"
	./charmrun +p20 ./message_sim


zerocopy:
	$(CHARMC) $(ZEROCOPY_DIR)/zerocopy.ci
	@mv zerocopy_sim.decl.h $(ZEROCOPY_DIR)/zerocopy_sim.decl.h
	@mv zerocopy_sim.def.h $(ZEROCOPY_DIR)/zerocopy_sim.def.h
	$(CHARMC) -o zerocopy_sim $(ZEROCOPY_DIR)/zerocopy.cc
	@echo "Built ping-pong using zerocopy"
	./charmrun +p20 ./zerocopy_sim


clean:
	rm -f *.decl.h *.def.h *.o charmrun message_sim zerocopy_sim


.PHONY : message zerocopy clean

