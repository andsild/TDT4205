TARGETS=$(shell ls *.vsl | sed s/\.vsl/\.table/g)
all: ${TARGETS}
%: %.s
%.table: %.vsl
	../src/vslc <$*.vsl > $*.table
clean:
	-rm -f *.table
