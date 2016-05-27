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
testa:
	./parser ../Test/a1.cmm ~/Documents/shares/irsim/test/a1_out.ir
	./parser ../Test/a2.cmm ~/Documents/shares/irsim/test/a2_out.ir
	./parser ../Test/a3.cmm ~/Documents/shares/irsim/test/a3_out.ir
	./parser ../Test/a4.cmm ~/Documents/shares/irsim/test/a4_out.ir
	./parser ../Test/a5.cmm ~/Documents/shares/irsim/test/a5_out.ir

testb:
	./parser ../Test/b1.cmm ~/Documents/shares/irsim/test/b1_out.ir
	./parser ../Test/b2.cmm ~/Documents/shares/irsim/test/b2_out.ir
	./parser ../Test/b3.cmm ~/Documents/shares/irsim/test/b3_out.ir
	./parser ../Test/b4.cmm ~/Documents/shares/irsim/test/b4_out.ir

testc:
	./parser ../Test/c1.cmm ~/Documents/shares/irsim/test/c1_out.ir
	./parser ../Test/c2.cmm ~/Documents/shares/irsim/test/c2_out.ir

testd:
	./parser ../Test/d1.cmm ~/Documents/shares/irsim/test/d1_out.ir
	./parser ../Test/d2.cmm ~/Documents/shares/irsim/test/d2_out.ir

teste:
	./parser ../Test/e4.cmm ~/Documents/shares/irsim/test/e4_out.ir
	./parser ../Test/e5.cmm ~/Documents/shares/irsim/test/e5_out.ir
	./parser ../Test/e6.cmm ~/Documents/shares/irsim/test/e6_out.ir


tt:
	./parser ../Test/e4.cmm ~/Documents/shares/irsim/test/t_out.ir

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
