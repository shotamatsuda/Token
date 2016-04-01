gulp = require 'gulp'
gutil = require 'gulp-util'
plumber = require 'gulp-plumber'
uglify = require 'gulp-uglify'
stylus = require 'gulp-stylus'
cssnano = require 'gulp-cssnano'
sourcemaps = require 'gulp-sourcemaps'
browserify = require 'browserify'
poststylus = require 'poststylus'
lost = require 'lost'
autoprefixer = require 'autoprefixer'
source = require 'vinyl-source-stream'
buffer = require 'vinyl-buffer'
browsersync = require('browser-sync').create()

# Configuration
paths =
  javascriptsEntryPoint: 'source/javascripts/all.coffee'
  javascripts: 'source/javascripts/**/*.coffee'
  stylesheetsEntryPoint: 'source/stylesheets/site.styl'
  stylesheets: 'source/stylesheets/**/*.styl'
  views: 'source/**/*.slim'
  bower: 'bower_components/**/*'
  dist: 'gulp_dist'

# Javascript task
gulp.task 'javascripts', ->
  browserify
      entries: [paths.javascriptsEntryPoint]
      debug: true
      extensions: ['.coffee']
      transform: ['coffeeify']
    .bundle()
    .pipe plumber()
    .pipe source('all.js') # to rename the resulting file
    .pipe buffer()  # to transform the browserify results into a stream
    .pipe sourcemaps.init()
    .pipe uglify
      sourceMap: true
    .pipe sourcemaps.write('.')
    .pipe gulp.dest(paths.dist + '/javascripts')
    .pipe browsersync.stream(match: '**/*.js')

# Stylesheet task
gulp.task 'stylesheets', ->
  gulp.src paths.stylesheetsEntryPoint
    .pipe plumber()
    .pipe sourcemaps.init()
    .pipe stylus
      use: [poststylus(['lost', 'autoprefixer'])]
      include: [paths.bower, 'source']
    .pipe cssnano()
    .pipe sourcemaps.write('.')
    .pipe gulp.dest(paths.dist + '/stylesheets')
    .pipe browsersync.stream(match: '**/*.css')

# Browsersync task
gulp.task 'browsersync', ->
  browsersync.init
    proxy: 'localhost:4567'

# Watch task
gulp.task 'watch', ['javascripts', 'stylesheets', 'browsersync'], ->
  gulp.watch paths.javascripts, ['javascripts']
  gulp.watch paths.stylesheets, ['stylesheets']
  gulp.watch paths.views
    .on 'change', browsersync.reload

# Default task
gulp.task 'default', ['javascripts', 'stylesheets']
