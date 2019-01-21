filein "mvd_utils.ms"
filein "mvd_managers.ms"

global selection_man = SelectionManager()

rollout selectionTools "Untitled" width:569 height:297
(
	GroupBox 'grp1' "Selection" pos:[13,10] width:434 height:175 align:#left
	dropdownList 'ddl1' "DropDownList" pos:[36,76] width:162 height:40 align:#left
	edittext 'edt1' "" pos:[33,51] width:168 height:21 align:#left
	edittext 'edt2' "" pos:[264,48] width:164 height:113 align:#left
	button 'btn1' "Select" pos:[37,132] width:163 height:30 align:#left
	
	-- Set the selected items given the inputs
	on btn1 pressed do (
		selection_man.triggerSelection()
	)

	-- On layer selection change, update my selection
	on ddl1 selected do (
		
	)
	
	-- Update the hierarchy when writting characters
	on edt1 changed do (
		
	)
)

function createRollouts = (
	
	dialog = newRolloutFloater "MVD Tools Exporter" 500 600
	
	addrollout selectionTools dialog rolledup:on 
)

createRollouts()
