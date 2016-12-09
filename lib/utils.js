'use strict';

module.exports = {
  v2_equals ( a, b, epsilon ) {
    let dx = b[0] - a[0];
    let dy = b[1] - a[1];

    let lenSqr = dx * dx + dy * dy;

    return lenSqr < epsilon * epsilon;
  },

  v3_equals ( a, b, epsilon ) {
    let dx = b[0] - a[0];
    let dy = b[1] - a[1];
    let dz = b[2] - a[2];

    let lenSqr = dx * dx + dy * dy + dz * dz;

    return lenSqr < epsilon * epsilon;
  },

  v3_dot ( a, b ) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  },
};
