################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --code_model=small -O0 --opt_for_speed=1 --use_hw_mpy=none --include_path="C:/ti/ccs1110/ccs/ccs_base/msp430/include" --include_path="C:/Users/Alex/workspace_v11/twine_FR4133" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --advice:power="3,4,5.2,5.3,6.2,6.3,7,8,9,10,11,12,15" --advice:hw_config=all --define=__MSP430FR4133__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


