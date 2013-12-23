COMPONENTS=test_gntshr_gnttab

.PHONY: all clean test

all:
	$(foreach COMPONENT, $(COMPONENTS), $(MAKE) -C $(COMPONENT);)

clean:
	$(foreach COMPONENT, $(COMPONENTS), $(MAKE) -C $(COMPONENT) clean;)

test: all
	@$(foreach COMPONENT, $(COMPONENTS), \
		echo -e "---------------------------------------------------" && \
		echo -e "Running program: $(COMPONENT)" && \
		echo -e "---------------------------------------------------" && \
		./$(COMPONENT)/$(COMPONENT) &&) :
