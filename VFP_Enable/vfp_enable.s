	AREA vfp_enable, CODE, READONLY

	EXPORT __enable_vfp [CODE]

	ENTRY
__enable_vfp
	MRC  p15,0,r0,c1,c0,2 ; Read CPACR into r0
	ORR  r0,r0,#(3<<20)   ; OR in User and Privileged access for CP10
	ORR  r0,r0,#(3<<22)   ; OR in User and Privileged access for CP11
	BIC  r0, r0, #(3<<30) ; Clear ASEDIS/D32DIS if set
	MCR  p15,0,r0,c1,c0,2 ; Store new access permissions into CPACR
	ISB                   ; Ensure side-effect of CPACR is visible
	MOV  r0,#(1<<30)      ; Create value with FPEXC (bit 30) set in r0
	VMSR FPEXC,r0         ; Enable VFP and SIMD extensions
	IMPORT __main
	B    __main           ; Enter normal C run-time environment & library start-up

	END


