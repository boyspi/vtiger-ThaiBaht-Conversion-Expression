<?php
/*+*******************************************************************************
 * The contents of this file are subject to the vtiger CRM Public License Version 1.0
 * ("License"); You may not use this file except in compliance with the License
 * The Original Code is:  vtiger CRM Open Source
 * The Initial Developer of the Original Code is vtiger.
 * Portions created by vtiger are Copyright (C) vtiger.
 * All Rights Reserved.
 ******************************************************************************/
require_once("include/events/SqlResultIterator.inc");
class VTExpressionsManager{
	function __construct($adb){
		$this->adb = $adb;
	}

	/** Caching logic **/
	private static $cache = array();
	static function addToCache($key, $value) {
		 self::$cache[$key] = $value;
	}
	static function fromCache($key) {
		if(isset(self::$cache[$key])) return self::$cache[$key];
		return false;
	}
	static function clearCache() {
		self::$cache = array();
	}
	/** END **/

	function fields($moduleName){
		global $current_user;
		$result = vtws_describe($moduleName, $current_user);
		$fields = $result['fields'];
		$arr = array();
		foreach($fields as $field){
			$arr[$field['name']] = $field['label'];
		}
		return $arr;
	}

	function expressionFunctions() {
		return array('concat' => 'concat(a,b)', 'time_diffdays' => 'time_diffdays(a,b)', 'time_diff' => 'time_diff(a,b)',
			'add_days' => 'add_days(datefield, noofdays)', 'sub_days' => 'sub_days(datefield, noofdays)',
			'get_date' => "get_date('today')",get_ThaiBahtConversion=>"get_ThaiBahtConversion(fieldname)");
	}
}

?>