CXX = g++
CXX_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline\
 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op\
 -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual\
 -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG\
 -D_EJUDGE_CLIENT_SIDE 

INC_DIR  = include/
SRC_DIR  = source/
OBJ_DIR  = object/
DMP_DIR  = gr_dump/
LOG_DIR  = log/

INCLUDES =  $(INC_DIR)list.h             \
            $(INC_DIR)hash_table.h       \
			$(INC_DIR)html_logfile.h     \
			$(INC_DIR)hash_functions.h   \

OBJECTS  =  $(OBJ_DIR)list.o             \
			$(OBJ_DIR)hash_table.o       \
			$(OBJ_DIR)html_logfile.o     \
			$(OBJ_DIR)hash_func_test.o   \
#			$(OBJ_DIR)debug_hash_table.o \
#			$(OBJ_DIR)test_list.o        \

DUMPS    =  $(DMP_DIR)*.dot              \
			$(DMP_DIR)*.png

LOGS     =  $(LOG_DIR)*.txt              \
			$(LOG_DIR)*.log              \
			$(LOG_DIR)*.html   

EXEC     =  test_list                    \
            test_hash_table              \
			test_hash_func               \

all: test_hash_func

test_list:        $(OBJECTS)
	$(CXX)        $(OBJECTS) -o $@ $(CXX_FLAGS)

debug_hash_table: $(OBJECTS)
	$(CXX)        $(OBJECTS) -o $@ $(CXX_FLAGS)

test_hash_func:   $(OBJECTS)
	$(CXX)        $(OBJECTS) -o $@ $(CXX_FLAGS)


$(OBJ_DIR)test_list.o:        $(SRC_DIR)test_list.cpp        $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)debug_hash_table.o: $(SRC_DIR)debug_hash_table.cpp $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)hash_func_test.o:   $(SRC_DIR)hash_func_test.cpp   $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)


$(OBJ_DIR)list.o:             $(SRC_DIR)list.cpp             $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)hash_table.o:       $(SRC_DIR)hash_table.cpp       $(INCLUDES) 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)html_logfile.o:     $(SRC_DIR)html_logfile.cpp     $(INCLUDES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)



.PHONY: clean clean_dumps clean_logs

clean:
	rm $(OBJECTS) $(EXEC)
	
clean_dumps:
	rm $(DUMPS)

clean_logs:
	rm $(LOG_DIR)*.html


DOT_DIR = dot_tests/

