# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -ll -ly

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
tt:
	./parser ../Test/t1.cmm ../Test/my_t1_out.s
	./parser ../Test/t2.cmm ../Test/my_t2_out.s
	./parser ../Test/t3.cmm ../Test/my_t3_out.s

testa:
	./parser ../Test/a1.cmm ../Test/a1_out.s
	./parser ../Test/a2.cmm ../Test/a2_out.s
	./parser ../Test/a3.cmm ../Test/a3_out.s
	./parser ../Test/a4.cmm ../Test/a4_out.s
	./parser ../Test/a5.cmm ../Test/a5_out.s

testb:
	./parser ../Test/b1.cmm ../Test/b1_out.s
	./parser ../Test/b2.cmm ../Test/b2_out.s
	./parser ../Test/b3.cmm ../Test/b3_out.s

testc:
	./parser ../Test/c1.cmm ../Test/c1_out.s
	./parser ../Test/c2.cmm ../Test/c2_out.s

testd:
	./parser ../Test/d1.cmm ../Test/d1_out.s
	./parser ../Test/d2.cmm ../Test/d2_out.s

test:
	./parser ../Test/t.cmm ../Test/my_t_out.s


clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
