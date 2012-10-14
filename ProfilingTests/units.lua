
Program {
	Name = "Example",
	Sources = {
		"Example.s",
		"TimingTools.c",
		"TimingMain.c",
	},
}

Program {
	Name = "EABehavior",
	Sources = {
		"EABehaviorAsm.s",
		"TimingTools.c",
		"EABehavior.c",
	},
}


Default "Example"
Default "EABehavior"
