CC = gcc
CFLAGS = -Wall
LIBS = -lpthread

TARGET = cv_mutex
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<  
  
# Generating dependency files
%.d : %.c
	@$(CC) -MM $< > $@
  
# Include dependency file to have gcc recompile necessary sources  
-include $(SRCS:%.c=%.d)
  
.PHONY: clean  
clean:  
	$(RM) *.o *.d $(TARGET)
