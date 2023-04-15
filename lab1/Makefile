# 定义编译器和编译选项
CC = gcc
CFLAGS = -Wall -g 
# -fsanitize=address


# 定义目标文件名
TARGETS = six.out

# 定义链接选项
LDFLAGS += -L/usr/local/glibc-debug/lib -Wl,--rpath=/usr/local/glibc-debug/lib
LDLIBS += -Wl,--dynamic-linker=/usr/local/glibc-debug/lib/ld-linux-x86-64.so.2 -lgcc_s -lc -lm -ldl

# 针对每个目标，生成对应的目标文件
$(TARGETS): %.out : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

# 默认目标
all: $(TARGETS)

# 清理目标
clean:
	rm -f $(TARGETS)
