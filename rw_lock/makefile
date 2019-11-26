CC = gcc
CFLAGS = -Wall

TARGET = pth_cv_mutex
SRCS = pth_cv_mutex.c my_rand.c
OBJS = $(SRCS:%.c=%.o)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ -lpthread

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

