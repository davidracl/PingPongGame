struct slidersPosition
{
	uint8_t left;
	uint8_t right;
};

struct slidersPosition get_sliders_position()
{
		struct adc_output newValues;	
		struct slidersPosition position;	
		newValues = adc_read();
		
		position.left = newValues.left_slider;
		position.right = newValues.right_slider;
		
		return position;
};