/* US translation for the jQuery Timepicker Addon */
(function($) {
$.datepicker.regional['us'] = {
	closeText: 'Fermer',
	prevText: '<',
	nextText: '>',
	isRTL: false,
	showMonthAfterYear: false,
	yearSuffix: ''
};
$.datepicker.setDefaults($.datepicker.regional['us']);
	$.timepicker.regional['us'] = {
		timeFormat: 'HH:mm',
		amNames: ['AM', 'A'],
		pmNames: ['PM', 'P'],
		isRTL: false
	};
	$.timepicker.setDefaults($.timepicker.regional['us']);
})(jQuery);
