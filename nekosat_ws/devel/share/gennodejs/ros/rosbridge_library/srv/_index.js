
"use strict";

let TestResponseOnly = require('./TestResponseOnly.js')
let TestRequestAndResponse = require('./TestRequestAndResponse.js')
let TestMultipleRequestFields = require('./TestMultipleRequestFields.js')
let TestNestedService = require('./TestNestedService.js')
let SendBytes = require('./SendBytes.js')
let TestMultipleResponseFields = require('./TestMultipleResponseFields.js')
let TestRequestOnly = require('./TestRequestOnly.js')
let TestArrayRequest = require('./TestArrayRequest.js')
let AddTwoInts = require('./AddTwoInts.js')
let TestEmpty = require('./TestEmpty.js')

module.exports = {
  TestResponseOnly: TestResponseOnly,
  TestRequestAndResponse: TestRequestAndResponse,
  TestMultipleRequestFields: TestMultipleRequestFields,
  TestNestedService: TestNestedService,
  SendBytes: SendBytes,
  TestMultipleResponseFields: TestMultipleResponseFields,
  TestRequestOnly: TestRequestOnly,
  TestArrayRequest: TestArrayRequest,
  AddTwoInts: AddTwoInts,
  TestEmpty: TestEmpty,
};
