# 编译器设置
CC = loongarch64-linux-gnu-gcc # 龙芯编译器
CFLAGS = -Wall -Wextra # 编译选项
TARGET = car # 目标文件
BUILD_DIR = build# 编译输出目录

# 源文件配置（确保目录不重叠）
SRC_DIRS := src include libs       # 只保留实际需要编译的目录
CFLAGS += $(addprefix -I, $(SRC_DIRS)) 

# 自动查找源文件（去重处理）
SRCS := $(shell find $(SRC_DIRS) -type f -name '*.c' | sort -u)

# 生成目标文件路径（保留目录结构）
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# 调试输出
# $(info Valid Sources: $(SRCS))
# $(info Objects: $(OBJS))

# 默认目标
all: $(TARGET) echo

# 链接规则
$(TARGET): $(OBJS)
	@$(CC) $^ -o $@ $(LDFLAGS)

# 精确编译规则
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MT $@ -c $< -o $@ 

# 依赖处理
-include $(DEPS)

clean:
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "-----------------------------------------------------------------------"
	@echo "---------------------------清理成功------------------------------------"

echo:
	@echo "-----------------------------------------------------------------------"
	@echo "---------------------------编译成功------------------------------------"

.PHONY: all clean echo