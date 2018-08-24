/* French translation for the jQuery Timepicker Addon */
(function($) {
$.datepicker.regional['fr'] = {
	closeText: 'Fermer',
	prevText: '<',
	nextText: '>',
	currentText: 'Courente',
	monthNames: ['Janvier','Frevrier','Mars','Avril','Mai','Juin',
	'Juillet','Aout','Septembre','Octobre','Novembre','Decembre'],
	monthNamesShort: ['Jan','Fev','Mars','Avr','Mai','Juin',
	'Juil','Aout','Sept','Oct','Nov','Dec'],
	dayNames: ['Dimanche','Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi'],
	dayNamesShort: ['Dim','Lun','Mar','Mer','Jeu','Ven','Sam'],
	dayNamesMin: ['Di','Lu','Ma','Me','Je','Ve','Sa'],
	weekHeader: 'Se',
	dateFormat: 'dd.mm.yy',
	firstDay: 1,
	isRTL: false,
	showMonthAfterYear: false,
	yearSuffix: ''
};
$.datepicker.setDefaults($.datepicker.regional['fr']);
	$.timepicker.regional['fr'] = {
		timeOnlyTitle: 'Choisir une heure',
		timeText: 'Heure',
		hourText: 'Heures',
		minuteText: 'Minutes',
		secondText: 'Secondes',
		millisecText: 'Millisecondes',
		timezoneText: 'Fuseau horaire',
		currentText: 'Maintenant',
		closeText: 'Valider',
		timeFormat: 'HH:mm',
		amNames: ['AM', 'A'],
		pmNames: ['PM', 'P'],
		isRTL: false
	};
	$.timepicker.setDefaults($.timepicker.regional['fr']);
})(jQuery);
