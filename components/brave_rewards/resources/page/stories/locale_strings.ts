/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { localeStrings as onboardingStrings } from '../../shared/components/onboarding/stories/locale_strings'

export const localeStrings = {
  ...onboardingStrings,

  appErrorTitle: 'Something went wrong',
  claim: 'Claim',
  connectWalletHeader: 'Choose an account provider',
  connectWalletDisclaimer: 'Custodial account providers are required to verify your identity, which may include submitting a photo ID. See the $1Brave Rewards Privacy Policy$2. See here to $3learn more about BAT$4.',
  connectWalletListItem1: 'Store, manage, and withdraw your BAT earnings',
  connectWalletListItem2: 'Top up your account with BAT you purchased',
  connectWalletListItem3: 'Send on-demand contributions to your favorite sites & creators',
  connectWalletLearnMore: 'Learn more about regions and support',
  connectWalletProviderNotAvailable: 'Currently not available in your region',
  rewardsAdGrantTitle: 'Your $1 Ad Rewards are here!',
  rewardsGrantDaysRemaining: 'You have $1 left to claim',
  rewardsLearnMore: 'Learn more',
  rewardsTokenGrantTitle: 'A token grant is available!',

  braveRewards: 'Brave Rewards',
  pendingContributions: 'Pending Contributions',
  donationAbility: 'Show Tip buttons on sites',
  donationTitle: 'Contributions',
  donationDesc: 'See the on-demand contributions you\'ve given to websites and creators.',
  donationDescLearnMore: 'Learn more about contributions.',
  donationVisitSome: 'Have you contributed to your favorite content creator today?',
  donationTotalDonations: 'Total on-demand contributions this month',
  earningsDisabledText: 'It looks like you’re unable to earn from Brave Ads. $1Learn more.$2',
  monthlyTipsEmpty: 'No monthly contributions set up yet.',
  monthlyTipsTitle: 'Monthly Contributions',
  monthlyTipsDesc: 'Set up recurring monthly contributions so you can continually support your favorite creators.',
  relaunch: 'Relaunch',
  on: 'on',
  site: 'Site',
  date: 'Date',
  tokens: 'Tokens',
  showAll: 'Show all',
  contributionVisitSome: 'Sites will appear as you browse',
  tosAndPp: 'By turning on $1, you agree to the $2Terms of Service$3 and $4Privacy Policy$5.',
  contributionTitle: 'Auto-Contribute',
  contributionDesc: 'An automatic way to support sites and content creators. Set a monthly contribution amount and browse normally. $1Verified sites and creators$2 you visit will receive contributions from you each month based on the amount of time you spend on their content.',
  contributionDisabledText1: 'Reward creators for the content you love.',
  contributionDisabledText2: 'Your monthly payment gets distributed across the sites you visit.',
  contributionMonthly: 'Monthly payment',
  contributionUpTo: 'Up to',
  contributionNextDate: 'Next contribution date',
  contributionSites: 'Sites viewed',
  nextContribution: 'Next contribution',
  rewardsContribute: 'Auto-Contribute',
  rewardsContributeAttention: 'Attention',
  contributionMinTime: 'Minimum page time before logging a visit',
  contributionTime5: '5 seconds',
  contributionTime8: '8 seconds',
  contributionTime60: '1 minute',
  contributionMinVisits: 'Minimum visits for publisher relevancy',
  contributionVisit1: '1 visit',
  contributionVisit5: '5 visits',
  contributionVisit10: '10 visits',

  adsSubdivisionTargetingDisabled: 'Disabled',
  adsSubdivisionTargetingDisable: 'Disable',
  adsSubdivisionTargetingAutoDetectedAs: 'Auto-detected as $1',
  adsSubdivisionTargetingAutoDetect: 'Auto-detect',
  adsSubdivisionTargetingTitle: 'Regional ad relevance',
  adsSubdivisionTargetingDescription: 'This allows Brave to show you ads meant specifically for your region.',
  adsSubdivisionTargetingLearn: 'Learn More',
  adsDescription: 'Control what kinds of Brave Ads you see, and how often.',
  adsDescriptionEarn: 'The more you see, the more you can earn.',
  adsPerHour: 'Notification ad frequency',
  adsPerHour0: 'None',
  adsPerHour1: 'Max 1 per hour',
  adsPerHour2: 'Max 2 per hour',
  adsPerHour3: 'Max 3 per hour',
  adsPerHour4: 'Max 4 per hour',
  adsPerHour5: 'Max 5 per hour',
  adsPerHour10: 'Max 10 per hour',
  adsTitle: 'Manage Brave Ads',
  adsCurrentEarnings: 'Earnings so far this month',
  adsPaymentDate: 'Next earnings payout date',
  adsTotalReceivedLabel: 'Total ads received this month',
  openAdsHistory: '30-day Ads History',
  settings: 'Settings',
  rewardsBrowserCannotReceiveAds: 'Oops! Your browser cannot receive Brave Private Ads.',
  rewardsBrowserNeedsUpdateToSeeAds: 'Your browser needs to be updated to continue seeing ads.',
  adsNotSupportedRegion: 'Sorry! Ads are not yet available in your region.',
  connectAccountText: '$1Ready to start earning?$2 Connect or create an account with one of our partners.',
  connectAccountNoProviders: 'To start earning BAT, you need to connect a custodial account to Brave Rewards. Unfortunately, there\'s no custodian available in your region, so earning isn\'t available at this time. However, you\'ll still be automatically supporting creators by using Rewards.',
  learnMore: 'Learn more',
  newTabAdCountLabel: 'New tab page ads',
  notificationAdCountLabel: 'Notification ads',
  newsAdCountLabel: 'Brave News ads',
  newsAdInfo: 'Brave News contains ads that cannot be independently disabled.',
  newsAdInfoDisabled: 'Brave News is currently disabled.',

  rewardsVBATNoticeTitle1: 'Action required: Connect a custodial account or your vBAT will be lost',
  rewardsVBATNoticeText1: 'On $1, we will be discontinuing support for existing virtual BAT balances. Connect a custodial account before this date so we can transfer your earned balance to your custodial account, otherwise your balance will be lost.',
  rewardsVBATNoticeTitle2: 'You still have time to contribute your vBAT to your favorite creators',
  rewardsVBATNoticeText2: 'On $1, we will be discontinuing support for existing virtual BAT balances. Unfortunately, there are no available custodians in your region ($2) to withdraw your earnings. Until then, you can still contribute to your favorite creators.',
  rewardsConnectAccount: 'Connect account',

  rewardsPaymentCompleted: 'The payout for $1 rewards has completed.',
  rewardsPaymentPending: 'The payout for $1 rewards will begin processing by $2',
  rewardsPaymentProcessing: 'The payout for $1 rewards is in progress.',
  rewardsPaymentSupport: 'Support'
}
