var gulp = require('gulp');
var gutil = require('gulp-util');
var uglify = require('gulp-uglify');
var stylus = require('gulp-stylus');
var cssnano = require('gulp-cssnano');
var sourcemaps = require('gulp-sourcemaps');
var browserify = require('browserify');
var poststylus = require('poststylus');
var source = require("vinyl-source-stream");
var buffer = require('vinyl-buffer');
var browserSync = require('browser-sync').create();

// Configuration
var paths = {
  'javascriptsEntryPoint': 'source/javascripts/all.coffee',
  'javascripts': 'source/javascripts/**/*.coffee',
  'stylesheetsEntryPoint': 'source/stylesheets/site.styl',
  'stylesheets': 'source/stylesheets/**/*.styl',
  'views': 'source/**/*.slim',
  'bower': 'bower_components',
  'tempDist': 'gulp_dist'
};
var postCssPlugins = [];

// Javascript task
gulp.task('javascripts', function() {
  browserify({
      entries: [paths.javascriptsEntryPoint],
      debug: true,
      extensions: [".coffee"],
      transform: ["coffeeify"]
    })
    .bundle()
    .pipe(source('all.js')) // to rename the resulting file
    .pipe(buffer())  // to transform the browserify results into a stream
    .pipe(sourcemaps.init())
    .pipe(uglify({
      sourceMap: true
    }))
    .pipe(sourcemaps.write('.'))
    .pipe(gulp.dest(paths.tempDist + '/javascripts'))
    .pipe(browserSync.stream({match: '**/*.js'}))
});

// Stylesheet task
gulp.task('stylesheets', function () {
  gulp.src(paths.stylesheetsEntryPoint)
    .pipe(sourcemaps.init())
    .pipe(stylus({
      use: [poststylus(postCssPlugins)],
      include: [paths.bower]
    }))
    .pipe(cssnano())
    .pipe(sourcemaps.write('.'))
    .pipe(gulp.dest(paths.tempDist + '/stylesheets'))
    .pipe(browserSync.stream({match: '**/*.css'}))
});

// Browsersync task
gulp.task('browser-sync', function(){
  browserSync.init({
    proxy: "localhost:4567"
  });
});

// Watch task
gulp.task('watch', ['javascripts', 'stylesheets', 'browser-sync'], function() {
  gulp.watch(paths.javascripts, ['javascripts']);
  gulp.watch(paths.stylesheets, ['stylesheets']);
  gulp.watch(paths.views).on('change', browserSync.reload);
});

// Default task
gulp.task('default', ['javascripts', 'stylesheets']);
