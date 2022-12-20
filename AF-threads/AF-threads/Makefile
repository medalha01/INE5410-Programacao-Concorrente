#Esse makefile só serve para fazer o arquivo a ser enviado pro moodle

.PHONY: submission clean

# Prepara .tar.gz pra submissão no moodle
# Note que antes de preparar o tar.gz, é feito um clean
submission: clean
	@rm -fr build $(OUTPUT)
	@SUBNAME=$$(basename "$$(pwd)"); \
		echo cd ..\; tar zcf "$$SUBNAME.tar.gz" "$$SUBNAME"
	@SUBNAME=$$(basename "$$(pwd)"); \
		cd ..; \
		rm -fr "$$SUBNAME.tar.gz"; \
		tar zcf "$$SUBNAME.tar.gz" "$$SUBNAME"
	@echo "Criado pacote \"$$(pwd).tar.gz\" para submissão"

# Limpa binários
clean:
	$(MAKE) -C exercicio_1             clean
	$(MAKE) -C exercicio_2             clean
	$(MAKE) -C exercicio_3             clean
#	$(MAKE) -C exercicio_4             clean
