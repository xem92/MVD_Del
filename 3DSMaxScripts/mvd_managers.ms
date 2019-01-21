-- Managers to handle extra functionality to max utils

struct SelectionManager 
(
	sm_layer = "0",
	sm_name = "",
	sm_candidates = #()
	
	-- Select the objects on the hierarchy panel
	fn triggerSelection = (
		
		-- Loop through all the candidates and set them as selection
		-- Select keyword to select the candidate nodes
	),
	
	-- By the given information (sm_layer & sm_name) update the hierarchy dropdownlist
	fn updateHierarchy = (
		
		-- Loop through all scene nodes
		-- Check if the layer of the current node is the same as the one set
		-- Check if the name of the node contains the substring set 
		-- Add it to the set if so
		
	),
	
	-- Update the current layer selection
	fn updateMenu = (
		
		-- Update the objects given the layer
		
	)
)