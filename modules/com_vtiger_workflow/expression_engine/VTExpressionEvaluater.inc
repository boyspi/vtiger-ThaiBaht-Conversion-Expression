<?php
/*+*******************************************************************************
 * The contents of this file are subject to the vtiger CRM Public License Version 1.0
 * ("License"); You may not use this file except in compliance with the License
 * The Original Code is:  vtiger CRM Open Source
 * The Initial Developer of the Original Code is vtiger.
 * Portions created by vtiger are Copyright (C) vtiger.
 * All Rights Reserved.
 ******************************************************************************/
interface VTExpressionEnv{
	function get($var);
}

function __vt_add($arr){
	if(sizeof($arr)==1){
		return $arr[0];
	}else{
		return $arr[0]+$arr[1];
	}
}

function __vt_sub($arr){
	if(sizeof($arr)==1){
		return -$arr[0];
	}else{
		return $arr[0]-$arr[1];
	}
}

function __vt_mul($arr){
	return $arr[0]*$arr[1];
}

function __vt_div($arr){
	try {
		return $arr[0]/$arr[1];
	}catch(Exception $e) {
		return 0;
	}
}

function __vt_equals($arr){
	return $arr[0] == $arr[1];
}

function __vt_ltequals($arr) {
	return $arr[0] <= $arr[1];
}

function __vt_gtequals($arr) {
	return $arr[0] >= $arr[1];
}

function __vt_lt($arr) {
	return $arr[0] < $arr[1];
}

function __vt_gt($arr) {
	return $arr[0] > $arr[1];
}

function __vt_concat($arr){
	return implode($arr);
}
/* Date difference between (input times) or (current time and input time)
 *
 * @param Array $a $a[0] - Input time1, $a[1] - Input time2
 * (if $a[1] is not available $a[0] = Current Time, $a[1] = Input time1)
 * @return int difference timestamp
 */

function __vt_time_diff($arr) {

    $time_operand1 = $time_operand2 = 0;
    if(count($arr) > 1) {
            $time_operand1 = $arr[0];
            $time_operand2 = $arr[1];
    } else {
            $time_operand1 = date('Y-m-d H:i:s'); // Current time
            $time_operand2 = $arr[0];
    }

    if(empty($time_operand1) || empty($time_operand2)) return 0;

    $time_operand1 = getValidDBInsertDateTimeValue($time_operand1);
	$time_operand2 = getValidDBInsertDateTimeValue($time_operand2);

    return (strtotime($time_operand1) - strtotime($time_operand2));
}
/**
 * Calculate the time difference (input times) or (current time and input time) and
 * convert it into number of days.
 * @param Array $a $a[0] - Input time1, $a[1] - Input time2
 * (if $a[1] is not available $a[0] = Current Time, $a[1] = Input time1)
 * @return int number of days
 */
function __vt_time_diffdays($arr) {
    $timediff  = __vt_time_diff($arr);
    $days_diff = floor($timediff / (60 * 60 * 24));
    return $days_diff;
}

function __vt_add_days($arr) {

	if (count($arr) > 1) {
		$baseDate = $arr[0];
		$noOfDays = $arr[1];
	} else {
		$noOfDays = $arr[0];
	}
	if($baseDate==null || empty($baseDate)) {
		$baseDate = date('Y-m-d'); // Current date
	}
	preg_match('/\d\d\d\d-\d\d-\d\d/', $baseDate, $match);
	$baseDate = strtotime($match[0]);
	$date = strftime('%Y-%m-%d', $baseDate + ($noOfDays * 24 * 60 * 60));
	return $date;
}

function __vt_sub_days($arr) {

	if (count($arr) > 1) {
		$baseDate = $arr[0];
		$noOfDays = $arr[1];
	} else {
		$noOfDays = $arr[0];
	}
	if($baseDate==null || empty($baseDate)) {
		$baseDate = date('Y-m-d'); // Current date
	}
	preg_match('/\d\d\d\d-\d\d-\d\d/', $baseDate, $match);
	$baseDate = strtotime($match[0]);
	$date = strftime('%Y-%m-%d', $baseDate - ($noOfDays * 24 * 60 * 60));
	return $date;
}

function __vt_get_date($arr) {
	$type = $arr[0];
	switch ($type) {
		case 'today': return date('Y-m-d');
			break;
		default : return date('Y-m-d');
			break;
	}
}


function __vt_get_ThaiBahtConversion($arr) {
	$amount_number = $arr[0];
	$amount_number = number_format($amount_number, 2, ".","");
	$pt = strpos($amount_number , ".");
	$number = $fraction = "";
	if ($pt === false)
		$number = $amount_number;
	else{
		$number = substr($amount_number, 0, $pt);
		$fraction = substr($amount_number, $pt + 1);
	}
       $ret = "";
	$baht = ReadNumber ($number);
	if ($baht != "")
		$ret .= $baht . "บาท";
 
	$satang = ReadNumber($fraction);
	if ($satang != "")
		$ret .=  $satang . "สตางค์";
	else
		$ret .= "ถ้วน";
	
	return $ret;
}

function ReadNumber($number)
{
    $position_call = array("แสน", "หมื่น", "พัน", "ร้อย", "สิบ", "");
    $number_call = array("", "หนึ่ง", "สอง", "สาม", "สี่", "ห้า", "หก", "เจ็ด", "แปด", "เก้า");
    $number = $number + 0;
    $ret = "";
    if ($number == 0) return $ret;
    if ($number > 1000000)
    {
        $ret .= ReadNumber(intval($number / 1000000)) . "ล้าน";
        $number = intval(fmod($number, 1000000));
    }
   
    $divider = 100000;
    $pos = 0;
    while($number > 0)
    {
        $d = intval($number / $divider);
        $ret .= (($divider == 10) && ($d == 2)) ? "ยี่" :
            ((($divider == 10) && ($d == 1)) ? "" :
            ((($divider == 1) && ($d == 1) && ($ret != "")) ? "เอ็ด" : $number_call[$d]));
        $ret .= ($d ? $position_call[$pos] : "");
        $number = $number % $divider;
        $divider = $divider / 10;
        $pos++;
    }
    return $ret;
} 


/** END * */
class VTFieldExpressionEvaluater{
	function __construct($expr){

		$this->operators = array(
				'+'  => '__vt_add',
				'-'  => '__vt_sub',
				'*'  => '__vt_mul',
				'/'  => '__vt_div',
				'==' => '__vt_equals',
				'<=' => '__vt_ltequals',
				'>=' => '__vt_gtequals',
				'<' => '__vt_lt',
				'>' => '__vt_gt',
		);
		$this->functions = array(
				'concat'=>'__vt_concat',
				'time_diff' => '__vt_time_diff',
 	            'time_diffdays' => '__vt_time_diffdays',
				'add_days' => '__vt_add_days',
				'sub_days' => '__vt_sub_days',
				'get_date' => '__vt_get_date',
				'get_ThaiBahtConversion' => '__vt_get_ThaiBahtConversion'
		);

		$this->operations = array_merge($this->functions, $this->operators);
		$this->expr = $expr;

	}

	function evaluate($env){
		$this->env = $env;
		return $this->exec($this->expr);
	}

	function exec($expr){
		if($expr instanceof VTExpressionSymbol){
			return $this->env($expr);
		}else if($expr instanceof VTExpressionTreeNode){
			$op = $expr->getName();
			if($op->value=='if'){
				$params = $expr->getParams();
				$cond = $this->exec($params[0]);
				if($cond){
					return $this->exec($params[1]);
				}else{
					return $this->exec($params[2]);
				}
			}else{
				$params = array_map(array($this, 'exec'), $expr->getParams());
				$func = $this->operations[$op->value];
				return $func($params);
			}
		}else{
			return $expr;
		}
	}

	function env($sym){
		return $this->env->get($sym->value);
	}
}
?>