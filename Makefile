include nemu/Makefile.git

default:
	@echo "Please run 'make' under any subprojects to compile."
 
clean:
	-$(MAKE) -C nemu clean
	-$(MAKE) -C nexus-am clean
	-$(MAKE) -C nanos-lite clean
	-$(MAKE) -C navy-apps clean

count:
	find . -name "*.h" -or -name "*.c" | xargs cat |wc -l

submit: clean
	git gc
	STUID=$(STUID) STUNAME=$(STUNAME) bash -c "$$(curl -s http://moon.nju.edu.cn/people/yyjiang/teach/submit.sh)"

.PHONY: default clean submit
