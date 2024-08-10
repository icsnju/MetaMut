// P2295R6 - Support for UTF-8 as a portable source file encoding
// This test intentionally contains various byte sequences which are not valid UTF-8
// { dg-do preprocess }
// { dg-options "-finput-charset=UTF-8 -Winvalid-utf8" }

// a߿ࠀ퟿𐀀􏿿a		{ dg-bogus "invalid UTF-8 character" }
// aa					{ dg-warning "invalid UTF-8 character <80>" }
// aa					{ dg-warning "invalid UTF-8 character <bf>" }
// aa					{ dg-warning "invalid UTF-8 character <c0>" }
// aa					{ dg-warning "invalid UTF-8 character <c1>" }
// aa					{ dg-warning "invalid UTF-8 character <f5>" }
// aa					{ dg-warning "invalid UTF-8 character <ff>" }
// aa					{ dg-warning "invalid UTF-8 character <c2>" }
// aa					{ dg-warning "invalid UTF-8 character <e0>" }
// aa				{ dg-warning "invalid UTF-8 character <e0><80><bf>" }
// aa				{ dg-warning "invalid UTF-8 character <e0><9f><80>" }
// aa					{ dg-warning "invalid UTF-8 character <e0><bf>" }
// aa					{ dg-warning "invalid UTF-8 character <ec><80>" }
// aa				{ dg-warning "invalid UTF-8 character <ed><a0><80>" }
// aa				{ dg-warning "invalid UTF-8 character <f0><80><80><80>" }
// aa				{ dg-warning "invalid UTF-8 character <f0><8f><bf><bf>" }
// aa				{ dg-warning "invalid UTF-8 character <f4><90><80><80>" }
// aa				{ dg-warning "invalid UTF-8 character <fd><bf><bf><bf>" }
//					{ dg-warning "invalid UTF-8 character <bf>" "" { target *-*-* } .-1 }
/* a߿ࠀ퟿𐀀􏿿a		{ dg-bogus "invalid UTF-8 character" } */
/* aa					{ dg-warning "invalid UTF-8 character <80>" } */
/* aa					{ dg-warning "invalid UTF-8 character <bf>" } */
/* aa					{ dg-warning "invalid UTF-8 character <c0>" } */
/* aa					{ dg-warning "invalid UTF-8 character <c1>" } */
/* aa					{ dg-warning "invalid UTF-8 character <f5>" } */
/* aa					{ dg-warning "invalid UTF-8 character <ff>" } */
/* aa					{ dg-warning "invalid UTF-8 character <c2>" } */
/* aa					{ dg-warning "invalid UTF-8 character <e0>" } */
/* aa				{ dg-warning "invalid UTF-8 character <e0><80><bf>" } */
/* aa				{ dg-warning "invalid UTF-8 character <e0><9f><80>" } */
/* aa					{ dg-warning "invalid UTF-8 character <e0><bf>" } */
/* aa					{ dg-warning "invalid UTF-8 character <ec><80>" } */
/* aa				{ dg-warning "invalid UTF-8 character <ed><a0><80>" } */
/* aa				{ dg-warning "invalid UTF-8 character <f0><80><80><80>" } */
/* aa				{ dg-warning "invalid UTF-8 character <f0><8f><bf><bf>" } */
/* aa				{ dg-warning "invalid UTF-8 character <f4><90><80><80>" } */
/* aa				{ dg-warning "invalid UTF-8 character <fd><bf><bf><bf>" } */
/*					{ dg-warning "invalid UTF-8 character <bf>" "" { target *-*-* } .-1 } */
